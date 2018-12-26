#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

pthread_mutex_t mutex;


/*此线程先获得互斥锁　然后停５秒　再释放*/
void* another( void* arg )
{
    printf( "in child thread, lock the mutex\n" );
    pthread_mutex_lock( &mutex );
    sleep( 5 );
    pthread_mutex_unlock( &mutex );
}

void prepare()
{
    pthread_mutex_lock( &mutex );
}

void infork()
{
    pthread_mutex_unlock( &mutex );
}

int main()
{
    pthread_mutex_init( &mutex, NULL );
    pthread_t id;
   // pthread_create( &id, NULL, another, NULL );
    pthread_atfork( prepare, infork, infork );
    
    /*睡眠一秒就是为了让线程先拿锁*/
    sleep( 1 );
    int pid = fork();
    if( pid < 0 )
    {
        pthread_join( id, NULL );
        pthread_mutex_destroy( &mutex );
        return 1;
    }
    
    /*子进程
     *子进程不会创建与父进程相同数量的线程　子进程只有一个执行线程
     *是对调用fork那个进线程的完整复制，　并且子进程自动的继承父进程中互斥锁(条件变量　信号量)的状态
     但是子进程并不知道是否有别的线进程是否对互斥锁的状态有所改变　如果子进程再枷锁就会产生死锁
    */
    else if( pid == 0 )
    {
        printf( "I am in the child, want to get the lock\n" );
        pthread_mutex_lock( &mutex );
        printf( "I can not run to here, oop...\n" );
        pthread_mutex_unlock( &mutex );
        exit( 0 );
    }

    /*父进程*/
    else
    {
        pthread_mutex_unlock( &mutex );
        wait( NULL );
    }

    pthread_join( id, NULL );
    pthread_mutex_destroy( &mutex );
    return 0;
}
