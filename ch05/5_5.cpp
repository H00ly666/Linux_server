/*************************************************************************
	> File Name: 5_5.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2017年12月17日 星期日 20时28分51秒
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
int  main(int argc,char *argv[])
{
    const char* ip = argv[1];
    int  port  = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock_id = socket( PF_INET,SOCK_STREAM,0 );
    assert(sock_id >= 0);
    
    int  ret = bind(sock_id,(struct sockaddr *)&address, sizeof(address) );
    assert(ret != -1);

    ret  = listen(sock_id,5);
    assert(ret != -1);
    
    sleep(5);
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof (client);

    int connfd = accept(sock_id, (struct sockaddr *)&client, &client_addrlength );
    if(connfd < 0)
        cout << "errno: " << errno ;
    else {
        char remote [INET_ADDRSTRLEN];
        cout << "connected with ip: " <<inet_ntop(AF_INET,&client.sin_addr,remote,INET_ADDRSTRLEN) << "and port" << 
        ntohs(client.sin_port);    
        close (connfd);
    }
    close (sock_id);
    return  0;


     




}
