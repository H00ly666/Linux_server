/*************************************************************************
	> File Name: EPOLLONESHOT.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2018年02月22日 星期四 14时51分03秒
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
    int epfd, nfds;
    char buf[256];
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events =EPOLLIN  |EPOLLONESHOT;  // LT是默认模式  EPOLLIN表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    while (1)                                               {
        nfds = epoll_wait(epfd, events, 5, -1);
        int i;
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                read(STDIN_FILENO, buf, sizeof(buf));
                printf("hello world\n");
            }
        //EPOLLONESHOT 模式下将其再次加入至监测中
        epfd = epoll_create(1);
        event.data.fd = STDIN_FILENO;
        event.events =EPOLLIN  |EPOLLONESHOT;  
        epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
        }
    }
}
