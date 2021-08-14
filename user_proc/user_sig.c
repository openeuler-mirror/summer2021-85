
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
 
#define USER_MSG    25
#define MSG_LEN     200

struct user_nl_msg
{
    struct nlmsghdr hdr;
    int8_t  data[MSG_LEN];
};
 
int main(int argc, char **argv)
{
    char *data = "hello kernel";
    struct sockaddr_nl  local, dest_addr;
 
    int skfd;
    struct nlmsghdr *nlh = NULL;
    struct user_nl_msg info;
    int ret;
 
    skfd = socket(AF_NETLINK, SOCK_RAW, USER_MSG);
    if(skfd == -1)
    {
        printf("create socket error...%s\n", strerror(errno));
        return -1;
    }
    
    // setting the socket structure
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
 
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; // to kernel
    dest_addr.nl_groups = 0;
 
    
    printf("wait kernel msg!\n");
    memset(&info, 0, sizeof(info));
	FILE *fp = NULL;
	
	// setting the time struct
	time_t rawtime; 
	struct tm *timeinfo; 
	time(&rawtime); 

	while(1){
		
		ret = recvfrom(skfd, &info, sizeof(struct user_nl_msg), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
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



