/*************************************************************************
	> File Name: 5_3.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年12月14日 星期四 11时42分57秒
 ************************************************************************/

#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
using namespace std;

static bool stop= false;
static void  handle_term(int  sig)
{
    stop = true;
}
        
int  main(int  argc,char *argv[])
{
    signal (SIGTERM,handle_term ); 

    if(argc<=3);
    const char* ip = argv[1];

    int port = atoi(argv[2]);//即将字符串转换成整形　函数
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET, SOCK_STREAM,0);
    assert(sock >=0);

    //创建一个ipv4　的socket地址
    struct sockaddr_in address;

    bzero(&address,sizeof(address)); //  
    /*原型：extern void bzero(void *s, int n);
　　用法：#include <string.h>
　　功能：置字节字符串s的前n个字节为零。
　　说明：bzero无返回值。
    * */
    
    address.sin_family = AF_INET;    //
    inet_pton (AF_INET,ip,&address.sin_addr); //
    address.sin_port = htons(port);  //host to network short    主机字节序(小端pc　) 转到 网络字节序(大段)
   //cout<< "########3" <<endl;
    int  ret = bind(sock,(struct sockaddr *)&address, sizeof (address) );

    assert (ret != -1);
    /*assert宏的原型定义在<assert.h>中，其作用是如果它的条件返回错误，则终止程序执行，原型定义：
    #include <assert.h>
    void assert( int expression );
    assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，
    然后通过调用 abort 来终止程序运行。 */ 
     ret = listen (sock,backlog);
    assert(ret != -1);
    
    //循还等待链接，直到有SIGTERM　信号将它中断
    while( !stop ){
        sleep(1);
    }
    close(sock);
    return 0;
}
