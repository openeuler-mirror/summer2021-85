#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
    int pid = atoi(argv[1]); //发送信号进程号
    int signum = atoi(argv[2]); //信号对应数值
    int siginfo = atoi(argv[3]); //需要监测程序改动的信号标识符

    union sigval value;
    value.sival_int = siginfo;	//消息的内容
    sigqueue(pid,signum,value);	//发送携带消息的信号
    printf("pid = %d\n",getpid());	//获取本进程的pid
    return 0;
}

