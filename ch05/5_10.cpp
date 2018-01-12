/*************************************************************************
	> File Name: 5_10.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2018年01月12日 星期五 14时20分33秒
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
using namespace std;

#define BUFFER_SIZE 512

int main ()
{
    


    const char *ip = argv[1];
    int port  =  atoi (argv[2]);
    
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address)  );
    
    inet_pton(AF_INET , ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);
    
    int sock = scoket(PF_INET , SOCK_STREAM ,0);
    assert (sock >= 0);

    int sendbuf = atoi(argv[3]);
    int  len = sizeof (sendbuf) ;

    //先设置tcp的发送缓冲区的大小 然后立即读取之
    setsockopt(sock, SOL_SOCKET , SO_SNDBUF , &sendbuf , sizeof(sendbuf));
    getsockopt(sock, SOL_SOCKET , SO_SNDBUF , &sendbuf , sizeof(sendbuf));
    cout << "the tcp send BUFFER size after setting  is "<< sendbuf<<endl;

    if(connect (sock , (struct sockaddr *)&server_address ,sizeof (server_address)) != -1  ){
        char  buffer [BUFFER_SIZE];
        memset( buffer , 'a' , BUFFER_SIZE );
        send (sock , buffer , BUFFER_SIZE , 0);

    }
    close (sock);
    return 0 ;





}

























