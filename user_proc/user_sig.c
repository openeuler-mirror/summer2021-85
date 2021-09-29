
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

// 定义netlink通信协议及传输设置
#define USER_MSG    25
#define MSG_LEN 200
#define MAX_PLOAD 100

// 信息传递数据结构
struct _my_msg
{
    struct nlmsghdr hdr;
    int8_t  data[MSG_LEN];
};

struct sockaddr_nl  local, dest_addr;
int skfd;
struct nlmsghdr *nlh = NULL;
struct _my_msg info;
char* data;

//信号捕获回调函数，向系统发送屏蔽信号数据
void handler(int signum,siginfo_t *info,void *flag)
{
    if(flag!=NULL)	//判断消息是否为空，不为空从info结构体中读出
    {
        printf("from pid : %d\n",info->si_pid);
        int sig_to_op = info->si_value.sival_int;
	    printf("sig_to_op : %d \n", sig_to_op);
        
	    char data[5];
        sprintf(data, "%d", sig_to_op);
	    printf("signum = %s\n", data);
        //清空上一次的通信数据
	    memset(NLMSG_DATA(nlh), 0, sizeof(NLMSG_DATA(nlh)));
        memcpy(NLMSG_DATA(nlh), data, strlen(data));
        int ret = sendto(skfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_nl));
        if(!ret)
        {
            perror("sendto error1\n");
            close(skfd);
            exit(-1);
        }
    }
}

int main(int argc, char **argv)
{
    int ret;

    skfd = socket(AF_NETLINK, SOCK_RAW, USER_MSG);
    if(skfd == -1)
    {
        printf("create socket error...%s\n", strerror(errno));
        return -1;
    }

    //初始化本地结构
    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_pid = 100; 
    local.nl_groups = 0;
    if(bind(skfd, (struct sockaddr *)&local, sizeof(local)) != 0)
    {
        printf("bind() error\n");
        close(skfd);
        return -1;
    }

    //初始化目的结构
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; // to kernel
    dest_addr.nl_groups = 0;

    //注册信号处理函数，接收到信号向系统发出信号集修改请求
    struct sigaction act;
    act.sa_sigaction = handler;	//绑定处理函数
    act.sa_flags = SA_SIGINFO;	//允许信号携带消息
    sigaction(SIGUSR1, &act, NULL);	//注册信号
 
    //发送初始化信号，系统初始化对应监测结构
    data = "0";
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
    memset(nlh, 0, sizeof(struct nlmsghdr));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_type = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = local.nl_pid; //self port

    memcpy(NLMSG_DATA(nlh), data, strlen(data));
    ret = sendto(skfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_nl));
    if(!ret)
    {
        perror("sendto error1\n");
        close(skfd);
        exit(-1);
    }

    printf("wait kernel msg!\n");
    memset(&info, 0, sizeof(info));
	FILE *fp = NULL;
	
	// 日志时间设置
	time_t rawtime;
	struct tm *timeinfo;

	while(1) {
		time(&rawtime);
        //清空上一次的通信数据
        memset(&info, 0 , sizeof(info));
		ret = recvfrom(skfd, &info, sizeof(struct _my_msg), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if(!ret)
		{
			perror("recv form kernel error\n");
			close(skfd);
			exit(-1);
		}
		fp = fopen("./log.txt", "a");
		timeinfo = localtime(&rawtime);

		char timestr[30];
		sprintf(timestr, "%s: ", asctime(timeinfo));
		timestr[strlen(timestr)-3] = ' ';
		fputs(timestr, fp);
		fputs(info.data, fp);
		fclose(fp);
	}
    close(skfd);
    free((void *)nlh);
    return 0;
}



