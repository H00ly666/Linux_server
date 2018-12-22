/*************************************************************************
	> File Name: 11-5tw_timer.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年12月22日 星期六 16时04分58秒
 ************************************************************************/

#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include"./11-5tw_timer.h"
 
using namespace std;
static time_wheel client_timer; 
void myfunc(client_data*arg)
{
 
	cout<<"timeout-->"<<"oldrotation:"<<arg->timer->oldrotation<<" now need rotation:"<<arg->timer->rotation<<" sockfd:"<<
			arg->sockfd<<" timer_slot:"<<arg->timer->time_slot<<" start_time:"<<arg->timer->start_time<<
			" expire:"<<arg->timer->expire<<" now:"<<time(NULL)<<" duration_time:"<<arg->timer->expire-arg->timer->start_time<<endl;
	arg =NULL;	
 
}
void* checktick(void*arg)
{
 
	while(1)
	{
	   client_timer.tick();
	   usleep(100);//后续采用信号处理
	}
	
}
int main()
 
{
 
   client_data*users = new client_data[20];
   srand(time(NULL));
   for(int i =0;i<10;i++)
  {
	users[i].sockfd = i;
	time_t cur_time = time(NULL);
	tw_timer* timer = client_timer.add_timer(cur_time+rand()%100+1);
	if(timer)
	{
		timer->user_data = &users[i];
		timer->cb_func = myfunc;
		timer->start_time = cur_time;
		users[i].timer = timer;
	}
 
 
 }
pthread_t pid;
pthread_create(&pid,NULL,checktick,NULL);
void* retval;
pthread_join(pid,&retval);
 
}

