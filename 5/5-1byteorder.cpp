#include <stdio.h>

/*
 * 判断大小端
 * char[0] 存01 char[1] 存02
 * 首先 test.value = 0x0102;
 * 接下来使用了test.union_bytes 
 * value 此时失效 union_bytes接管value 的数据
 */
//void byteorder()
int main()
{
	union
	{
		short value;
		char union_bytes[ sizeof( short ) ];
	} test;
	test.value = 0x0102;
	if (  ( test.union_bytes[ 0 ] == 1 ) && ( test.union_bytes[ 1 ] == 2 ) )
	{
		printf( "big endian\n" );
	}
	else if ( ( test.union_bytes[ 0 ] == 2 ) && ( test.union_bytes[ 1 ] == 1 ) )
	{
		printf( "little endian\n" );
	}
	else
	{
		printf( "unknown...\n" );
	}
}
