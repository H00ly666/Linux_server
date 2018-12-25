#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*利用辅助数据在进程中传递文件描述符
* 目前对据体育安利不详　应该是使用使用管道来实现
* 然后使用　sendmsg 这个在进程之间收发数据　*/


static const int CONTROL_LEN = CMSG_LEN( sizeof(int) );

void send_fd( int fd, int fd_to_send )
{
    struct iovec iov[1];
    /*数据msghdr适用于通用scoket借口sendmsg*/
    struct msghdr msg;
    char buf[0];

    iov[0].iov_base = buf;
    iov[0].iov_len = 1;
    
    /*填充数据*/
    msg.msg_name    = NULL;  //协议名　TCP 为NULL
    msg.msg_namelen = 0;     //NULL自然是0
    msg.msg_iov     = iov;   //数据块数组赋值　实现集中写　离散读
    msg.msg_iovlen = 1;　　　//数据块数量

    /*辅助数据包　目前作用不详　数据完全可以写在内存块中*/
    cmsghdr cm;
    cm.cmsg_len = CONTROL_LEN;
    cm.cmsg_level = SOL_SOCKET;
    cm.cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA( &cm ) = fd_to_send;
    
    msg.msg_control = &cm;
    msg.msg_controllen = CONTROL_LEN;

    sendmsg( fd, &msg, 0 );
}

int recv_fd( int fd )
{
    struct iovec iov[1];
    struct msghdr msg;
    char buf[0];

    /*和上边sendmsg道理相似　不过是解析出来而已*/
    iov[0].iov_base = buf;
    iov[0].iov_len = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov     = iov;
    msg.msg_iovlen = 1;

    cmsghdr cm;
    msg.msg_control = &cm;
    msg.msg_controllen = CONTROL_LEN;

    recvmsg( fd, &msg, 0 );

    int fd_to_read = *(int *)CMSG_DATA( &cm );
    return fd_to_read;
}

int main()
{
    int pipefd[2];
    int fd_to_pass = 0;

    int ret = socketpair( PF_UNIX, SOCK_DGRAM, 0, pipefd );
    assert( ret != -1 );

    pid_t pid = fork();
    assert( pid >= 0 );

    if ( pid == 0 )
    {
        /*在子进程中发送套接字*/
        close( pipefd[0] );
        fd_to_pass = open( "./test.c", O_RDWR, 0666 );
        /*错误发生标准输入*/
        send_fd( pipefd[1], ( fd_to_pass > 0 ) ? fd_to_pass : 0 );
        close( fd_to_pass );
        exit( 0 );
    }

    close( pipefd[1] );
    fd_to_pass = recv_fd( pipefd[0] );
    
    char buf[1024];
    memset( buf, '\0', 1024 );
    /*进行数据验证/
    read( fd_to_pass, buf, 1024 );
    printf( "I got fd %d and data %s\n", fd_to_pass, buf );
    close( fd_to_pass );


}
