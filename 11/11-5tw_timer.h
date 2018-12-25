#ifndef TIME_WHEEL_TIMER
#define TIME_WHEEL_TIMER

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE 64
class tw_timer;
/* 客户端类绑定socket和定时器 */
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[ BUFFER_SIZE ];
    tw_timer* timer;
};

/* 定时器类 */
class tw_timer
{
public:
    tw_timer( int rot, int ts ) 
    : next( NULL ), prev( NULL ), rotation( rot ), time_slot( ts ){}

public:
    /* 记录定时器在实践论旋转多少圈后生效 */
    int rotation;
    /* 记录定时器属于那个时间槽 */
    int time_slot;
    /* 定时器回调函数 */
    void (*cb_func)( client_data* );
    /* 客户数据 */
    client_data* user_data;
    tw_timer* next;
    tw_timer* prev;
};

/* 时间轮 */
class time_wheel
{
public:
    /* 初始化每个槽的头节点 设置为null*/
    time_wheel() : cur_slot( 0 )
    {
        for( int i = 0; i < N; ++i )
        {
            slots[i] = NULL;
        }
    }
    /*遍历槽 进入其中每一个定时器节点 并进行销毁 */
    ~time_wheel()
    {
        for( int i = 0; i < N; ++i )
        {
            tw_timer* tmp = slots[i];
            while( tmp )
            {
                slots[i] = tmp->next;
                delete tmp;
                tmp = slots[i];
            }
        }
    }
    /* 根据定时值timeout创建一个定时器 并把它插入合适的槽中 */
    tw_timer* add_timer( int timeout )
    {
        if( timeout < 0 )
        {
            return NULL;
        }
        
        int ticks = 0;
        
        /*下面根据带插入定时器的超时值计算它将在时间轮转动多少个滴答之后被触发
         *并将该数存于ticks中 如过小于事件槽间隔数TI 则将ticks向上折合为1 否则就将
         *ticks 向下折合为timeout/TI */
        if( timeout < TI )
        {
            ticks = 1;
        }
        else
        {
            ticks = timeout / TI;
        }
        /*计算待插入的定时器在事件轮转动多少圈之后触发*/
        int rotation = ticks / N;
        /*计算定时器应该被插入那个槽中*/
        int ts = ( cur_slot + ( ticks % N ) ) % N;
        /*创建新的定时器 它在时间轮转动rotation之后被触发 且位于第ts个槽上 */
        tw_timer* timer = new tw_timer( rotation, ts );
        
        /*如果第ts个槽上无任何定时器 则把新建的定时器 插入其中 并将该定时器设置为该槽的头结点*/
        if( !slots[ts] )
        {
            printf( "add timer, rotation is %d, ts is %d, cur_slot is %d\n", rotation, ts, cur_slot );
            slots[ts] = timer;
        }
        /*否则将定时器插入第ts个槽中*/
        else
        {
            timer->next = slots[ts];
            slots[ts]->prev = timer;
            slots[ts] = timer;
        }
        return timer;
    }
    
    void del_timer( tw_timer* timer )
    {
        if( !timer )
        {
            return;
        }
        int ts = timer->time_slot;
        if( timer == slots[ts] )
        {
            slots[ts] = slots[ts]->next;
            if( slots[ts] )
            {
                slots[ts]->prev = NULL;
            }
            delete timer;
        }
        else
        {
            timer->prev->next = timer->next;
            if( timer->next )
            {
                timer->next->prev = timer->prev;
            }
            delete timer;
        }
    }
    void tick()
    {
        tw_timer* tmp = slots[cur_slot];
        printf( "current slot is %d\n", cur_slot );
        while( tmp )
        {
            printf( "tick the timer once\n" );
            if( tmp->rotation > 0 )
            {
                tmp->rotation--;
                tmp = tmp->next;
            }
            else
            {
                tmp->cb_func( tmp->user_data );
                if( tmp == slots[cur_slot] )
                {
                    printf( "delete header in cur_slot\n" );
                    slots[cur_slot] = tmp->next;
                    delete tmp;
                    if( slots[cur_slot] )
                    {
                        slots[cur_slot]->prev = NULL;
                    }
                    tmp = slots[cur_slot];
                }
                else
                {
                    tmp->prev->next = tmp->next;
                    if( tmp->next )
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    tw_timer* tmp2 = tmp->next;
                    delete tmp;
                    tmp = tmp2;
                }
            }
        }
        cur_slot = ++cur_slot % N;
    }

private:
    /* 时间轮上槽的数目 */
    static const int N = 60;
    /* 每一秒时间轮转动一次 即时间槽的间隔是一秒钟 */
    static const int TI = 1; 
    /* 时间轮的槽，其中每个元素指向一个定时器链表 链表无序 */
    tw_timer* slots[N];
    /* 时间轮的当前槽 */
    int cur_slot;
};

#endif
