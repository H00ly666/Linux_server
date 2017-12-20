/*************************************************************************
	> File Name: 5-6.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2017年12月20日 星期三 21时12分22秒
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
int main ()
{
    const *ip = argv[1];
    int  port = atoi(argv[2]);

    struct sockaddr_in   server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip , &server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr *)&server_address,sizeof(server_address) );

    const char * oob_data = "abc";
    const char * normal_data = "123";
    send(sockfd , normal_data, strlen(normal_data), 0 );
    send(sockfd , oob_data , strlen(oob_data), MSG_OOB );
    send(sockfd, normal_data , strlen(normal_data), 0 );

    close(sockfd);
    return 0;



}
