#ifndef intIME_HEAP
#define intIME_HEAP

#include <iostream>
#include <netinet/in.h>
#include <time.h>
using std::exception;

#define BUFFER_SIZE 64

class heap_timer;
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[ BUFFER_SIZE ];
    heap_timer* timer;
};
/*定时器类*/
class heap_timer
{
public:
    heap_timer( int delay )
    {
        /* 延迟处理 */
        expire = time( NULL ) + delay;
    }

public:
    /* 定时器生效的绝对时间 */
    time_t expire;
    /*定时器回掉函数*/
    void (*cb_func)( client_data* );
    client_data* user_data;
};
/*时间堆类*/
class time_heap
{
public:
    /*构造函数之一 初始化一个大小为cap 的空堆*/
    time_heap( int cap ) throw ( std::exception )
        : capacity( cap ), cur_size( 0 )
    {
	    array = new heap_timer* [capacity];
	    if ( ! array )
	    {
                throw std::exception();
	    }
            for( int i = 0; i < capacity; ++i )
        {
            array[i] = NULL;
        }
    }
    /*用已有的数组来初始化堆*/
    time_heap( heap_timer** init_array, int size, int capacity ) throw ( std::exception )
        : cur_size( size ), capacity( capacity )
    {
        if ( capacity < size )
        {
            throw std::exception();
        }
        array = new heap_timer* [capacity];
        if ( ! array )
        {
            throw std::exception();
        }
        for( int i = 0; i < capacity; ++i )
        {
            array[i] = NULL;
        }
        /* 初始化堆数组 */
        if ( size != 0 )
        {
            for ( int i =  0; i < size; ++i )
            {
                array[ i ] = init_array[ i ];
            }
            for ( int i = (cur_size-1)/2; i >=0; --i )
            {
                /* 执行下虑操作 */
                percolate_down( i );
            }
        }
    }
    /*销毁时间堆*/
    ~time_heap()
    {
        for ( int i =  0; i < cur_size; ++i )
        {
            delete array[i];
        }
        delete [] array; 
    }

public:
    /*添加目标定时器timer*/
    void add_timer( heap_timer* timer ) throw ( std::exception )
    {
        if( !timer )
        {
            return;
        }
        /*容量不够 重新来分配堆数组  既然使用了c++为什么不使用vector来避免这个问题*/
        if( cur_size >= capacity )
        {
            resize();
        }
        int hole = cur_size++;
        int parent = 0;
        /* 从下进行上虑 */
        for( ; hole > 0; hole=parent )
        {
            parent = (hole-1)/2;
            if ( array[parent]->expire <= timer->expire )
            {
                break;
            }
            array[hole] = array[parent];
        }
        array[hole] = timer;
    }
    /* 删除目标定时器 */
    void del_timer( heap_timer* timer )
    {
        if( !timer )
        {
            return;
        }
        // lazy delelte
        /*这里比较有意思 只将目标定时器的回调函数置为空
        * 即所谓的延时销毁
        * 这将真正节省删除该定时器造成的开销
        * 但这样做容易使数组膨胀*/
        timer->cb_func = NULL;
    }
    
    /* 获得堆顶部的定时器 */
    heap_timer* top() const
    {
        if ( empty() )
        {
            return NULL;
        }
        return array[0];
    }
    /*删除堆顶部的定时器*/
    void pop_timer()
    {
        if( empty() )
        {
            return;
        }
        if( array[0] )
        {
            delete array[0];
            /*将原来的堆顶元素替换为堆数组中最后一个元素*/
            array[0] = array[--cur_size];
            /*对新的堆顶元素进行下虑操作*/
            percolate_down( 0 );
        }
    }

    /*心博函数*/
    void tick()
    {
        heap_timer* tmp = array[0];
        time_t cur = time( NULL );
        while( !empty() )
        {
            if( !tmp )
            {
                break;
            }
            if( tmp->expire > cur )
            {
                break;
            }
            if( array[0]->cb_func )
            {
                array[0]->cb_func( array[0]->user_data );
            }
            pop_timer();
            tmp = array[0];
        }
    }
    bool empty() const { return cur_size == 0; }

private:
    void percolate_down( int hole )
    {
        heap_timer* temp = array[hole];
        int child = 0;
        for ( ; ((hole*2+1) <= (cur_size-1)); hole=child )
        {
            child = hole*2+1;
            if ( (child < (cur_size-1)) && (array[child+1]->expire < array[child]->expire ) )
            {
                ++child;
            }
            if ( array[child]->expire < temp->expire )
            {
                array[hole] = array[child];
            }
            else
            {
                break;
            }
        }
        array[hole] = temp;
    }
    /* 将数组容量扩大一倍 */
    void resize() throw ( std::exception )
    {
        heap_timer** temp = new heap_timer* [2*capacity];
        for( int i = 0; i < 2*capacity; ++i )
        {
            temp[i] = NULL;
        }
        if ( ! temp )
        {
            throw std::exception();
        }
        capacity = 2*capacity;
        for ( int i = 0; i < cur_size; ++i )
        {
            temp[i] = array[i];
        }
        delete [] array;
        array = temp;
    }

private:
    /*动态数组 这里采用 vector容器是不是更好一些呢 */
    heap_timer** array;
    /*最小堆数组的容量*/
    int capacity;
    /*堆数组当前包含元素的个数*/
    int cur_size;
};

#endif
