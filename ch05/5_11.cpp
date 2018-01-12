/*************************************************************************
	> File Name: 5_11.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2018年01月12日 星期五 14时32分52秒
 ************************************************************************/

#include<iostream>
#include<netinet/in.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<assert.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define BUFFER_SIZE 1024

using namespace std;


int main(int argc ,char ** argv)
{
    const char * ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address; 
    bzero (&address, sizeof(address));

    address.sin_family = AF_INET; 
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock= socket (PF_INET , SOCK_STREAM, 0);
    assert(sock >=0);
    int recvbuf= atoi (argv[3]);
    int len = sizeof(recvbuf);
    //先设置TCP接受缓冲区的大小,然后立即读取之
    //
    setsockopt(sock, SOL_SOCKET, SO_RECVBUF , &recvbuf, sizeof(recvbuf));
    getsockopt(sock, SQL_SOCKET ,SO_RECVBUF, &recvbuf, (socklen_t*)&len );

    cout << "the tcp receive buffer size after setting is  "<< recvbuf <<endl;

    int ret  = bind  (sock, (struct sockaddr*)&address , sizeof(address));
    assert (ret != -1);
    ret  = listen (sock , 5);
    assert (ret != -1);
    struct scockaddr_in client ;
    socklen_t client_addrlength = sizeof(client);

    int connfd (sock, (struct sockaddr*)&client &client_addrlength);
    if(connfd < 0){
        cout << "error is "<< errno<< endl;
    }else{
        char buffer [BUFFER_SIZE];
        memset( buffer ,'\0',BUFFER );
        while(recv (connfd ,buffer , BUFFER_SIZE-1, 0)> 0 );
        close (connfd);
    
    }
    close (sock);
    return 0;

}
