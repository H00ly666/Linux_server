#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <iostream>
using namespace std ;
int main()
{
    int epfd, nfds;
    char buf[256];
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events =EPOLLIN;  // LT是默认模式  EPOLLIN表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    while (1) {
        nfds = epoll_wait(epfd, events, 5, -1);
        int i;
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
               cout<<"#####" << endl; 
                 read(STDIN_FILENO, buf, sizeof(buf));  //read  会把缓冲区的数据拿掉  epoll一直在检测  发现数据从无到有时间
                printf("hello world\n");
            }
        }
    }
}
