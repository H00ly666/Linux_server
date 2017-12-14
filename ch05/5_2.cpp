/*************************************************************************
	> File Name: 5_2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年12月14日 星期四 11时33分17秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<arpa/inet.h>
using namespace std;
int  main()
{
    char *v1 = inet_ntoa("1.2.3.4");
 //   char *v2 = inet_ntoa("10.194.71.60");
    cout << "address1 :"<< v1<<endl;
 //   cout << "address2 :"<< v2<<endl;

}
