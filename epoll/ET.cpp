/*************************************************************************
	> File Name: ET.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2018年02月22日 星期四 13时00分33秒
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
#include<sys/epoll.h>
using namespace std;
int main()
{
    int  epfd, nfds;
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN | EPOLLET;//边缘触发

    epoll_ctl(epfd, EPOLL_CTL_ADD,STDIN_FILENO, &event);
    while(1){
        cout << ( nfds = epoll_wait(epfd, events, 5 , 500) ) <<endl ;
        
        int i ;
        for(i = 0 ; i < nfds; ++i ){
            if(events[i].data.fd == STDIN_FILENO){
                cout << "hello world!" << endl;
               // event.data.fd = STDIN_FILENO;
               // event.events = EPOLLIN | EPOLLET;
               // epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
            }    
        }
    }
}


