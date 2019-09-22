/*************************************************************************
	> File Name: 5_1.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年12月14日 星期四 11时12分01秒
 ************************************************************************/

#include<iostream>
using namespace std;
int main()
{
    union {
        short value;
        char  union_bytes[sizeof(short)];
    }test;
    
    test.value = 0x0102; // 258 存计算机16进制转为二进制
    //一个字节八｀   
    if((test.union_bytes[0]) == 1  && (test.union_bytes[1]) == 2   )
    {
        printf("big endian\n");
    }else if( (test.union_bytes[0]) == 2  && (test.union_bytes[1]) == 1 ){
        printf("little endian\n");
    }else{
        printf("unknown\n");
    }
//大段小端这一块一直感觉没有什么用  今天才感觉到的
}
