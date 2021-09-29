// test demo for openeuler signal monitor
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
int m_switch = 1;
void sig_usr_close(int sig);
void sig_usr_open(int sig);
 
int main()
{
    if(signal(SIGUSR1, sig_usr_close) == SIG_ERR)
    {
        printf("signal error\n");
    }
    if(signal(SIGUSR2, sig_usr_open) == SIG_ERR)
    {
        printf("signal error\n");
    }
    printf("%d\n", getpid());
    
    while(1)
    {
        if(m_switch == 1)
        {
            printf("hello\n");
        }
        sleep(8);
    }
    return 0;
}
void sig_usr_close(int sig)
{
    printf("reveive SIGUSER1\n");
    m_switch = 0;
}
void sig_usr_open(int sig)
{
    printf("reveive SIGUSER2\n");
    m_switch = 1;
}