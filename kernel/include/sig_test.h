//zeyu test

#include <linux/init.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>

#ifndef _SIG_TEST_H
#define _SIG_TEST_H

#define NETLINK_SIG_MONITOR 25

#define MAX_MSGSIZE 100

#define SIG_RECV_PID 100

extern struct sock *sig_monitor_sk;

extern struct net init_net;

extern char sig_send_buf[MAX_MSGSIZE];

extern int netlink_sig_init(void);

extern void prepare_for_nt_send(void);

extern void send_sig_info_to_user(struct ksignal *ksig);

extern void send_nlmsg(int8_t *message, uint16_t len, int pid);

extern struct netlink_kernel_cfg sig_cfg;

extern void recv_callback(struct sk_buff *skb);


#endif
