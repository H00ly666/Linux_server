/*************************************************************************
	> File Name: getaddrinfo.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年12月20日 星期四 17时26分50秒
 ************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(void)
{
    char               buf[100];
    int                ret = 0;
    struct addrinfo    hints;
    struct addrinfo    *res, *curr;
    struct sockaddr_in *sa;
 
    bzero(&hints, sizeof (hints));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_INET;
    /* 获取主机名 */
    if (gethostname(buf, sizeof (buf)) < 0) {
        perror("gethostname");
        return -1; 
    }
    printf ("hostname =%s\n", buf);
    /*
     * 这个函数比较有意思 网上好多博客说这个函数不支持ipv6 
     * 此函数返回的result 是一个链表那就是说会有多个返回值的可能了
     * 照此看来是又必要建立多个套接字进行管理 
     */
    if((ret = getaddrinfo(buf, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return -1; 
    }
    curr = res;
    while (curr && curr->ai_canonname) {
        sa = (struct sockaddr_in *)curr->ai_addr;
        printf("name: %s\nip: %s\n port: %d\n", curr->ai_canonname, 
                inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof (buf)),
                sa->sin_port);   
        curr = curr->ai_next;
    }
    /*
     * name: liujiahui-PC
     * ip: 127.0.0.1
     *
     */
}
