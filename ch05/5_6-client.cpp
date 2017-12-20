/*************************************************************************
	> File Name: 5_6-client.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2017年12月20日 星期三 21时26分14秒
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
int main (int argc,char **argv)
{
    const  char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address , sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int sockfd = socket (PF_INET , SOCK_STREAM,0 );
    bind(sockfd , (struct sockaddr *)&address ,sizeof(address) );
    listen(sockfd, 5);

    struct sockaddr_in client;


}
