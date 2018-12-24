#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

union semun
{
     int val;                  
     struct semid_ds* buf;     
     unsigned short int* array;
     struct seminfo* __buf;    
};

void pv( int sem_id, int op )
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = op;
    sem_b.sem_flg = SEM_UNDO;
    semop( sem_id, &sem_b, 1 );
}

int main( int argc, char* argv[] )
{
    /*创建一个信号量集 键值 数目 标志/权限 */
    int sem_id = semget( IPC_PRIVATE, 1, 0666 );
    /*
     * union semun
     * {
     *   int val;                 //用于SETVAL命令
     *   struct semid_ds * buf;   //用于IPC_STST 和 IPC_SET
     *   unsigned short* array;   //用于GETALL 和 SETALL 命令
     *   struct seminfo* _buf;    //用于IPC_INFO
     * };
     *
     * struct seminfo
     * {
     *   int semmap;   
     *
     *
     * }
     *
     *
     *
     *
     */

    union semun sem_un;
    sem_un.val = 1;

    /* 对信号量进行控制 标识　具体信号量在集中编号　命令 */
    semctl( sem_id, 0, SETVAL, sem_un );

    pid_t id = fork();
    if( id < 0 )
    {
        return 1;
    }
    //子进程
    else if( id == 0 )
    {
        printf( "child try to get binary sem\n" );
        /*在父子进程共享IPC_PRIVATE信号量的关键在于二者都可以操作该信号量的标识符sem_id*/
        pv( sem_id, -1 );
        printf( "child get the sem and would release it after 5 seconds\n" );
        sleep( 5 );
        pv( sem_id, 1 );
        exit( 0 );
    }
    else
    {
        printf( "parent try to get binary sem\n" );
        pv( sem_id, -1 );
        printf( "parent get the sem and would release it after 5 seconds\n" );
        sleep( 5 );
        pv( sem_id, 1 );
    }

    waitpid( id, NULL, 0 );
    /*删除信号量*/
    semctl( sem_id, 0, IPC_RMID, sem_un );
    return 0;
}
