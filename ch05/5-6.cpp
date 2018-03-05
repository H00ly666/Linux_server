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

void * client_web(void * arg)
{
    
    cout<< " 你好啊"<<endl;
    
    char str;
    
    struct sockaddr_in servaddr; 
    unsigned int client_fd = 0xffff;  
    client_fd &= (long long)arg; 
    
    
    /*正常的 HTTP header */
    while(1){
        recv( client_fd, &str ,1,0 );
        cout << str << endl ;
    
    }

    cout<< " 你好啊"<<endl;
}

int main (int argc , char** argv  )
{
    
    char  ip[20] = "127.0.0.1";
    int  port = 8888;
    
    pthread_t   thread;

    struct sockaddr_in   server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip , &server_address.sin_addr);
    server_address.sin_port = htons(port);
    //创建套接字
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    //绑定
    int a =  bind(sockfd,(struct sockaddr * ) &server_address , sizeof(server_address)) ;
    //开始监听
    listen(sockfd,100);
    
    //###################################
    cout << "wait for web get" <<endl;

    while(1){
        int connfd =  accept(sockfd, NULL, NULL);
        cout <<  "接受到的"<< connfd <<endl;
        //开线程处理
        if(pthread_create(&thread,NULL, client_web , (void *) connfd ) < 0)       
            printf("create error\n");

    }



    close(sockfd);
    return 0;



}
