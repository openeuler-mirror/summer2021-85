#include<unistd.h>
#include<stdio.h>
#include<signal.h>

typedef void (*sighandler_t)(int);//函数指针
sighandler_t old_handler = NULL;

void myhandler(int signo)
{
      printf("got SIGINT\n");//第一次按ctrlC捕捉2号信号SIGINT
      signal(2, old_handler);//恢复默认的处理，再按ctrl C的话，就会终止程序
}
int main()
{
   old_handler = signal(2,myhandler);//捕捉2号信号SIGINT：Ctrl C
   while(1)
   {
       sleep(1);
   }
   return 0;
}