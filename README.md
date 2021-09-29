# Summer2021-No.85 openEuler异常信号监测

#### 介绍
https://gitee.com/openeuler-competition/summer-2021/issues/I3HHH2

#### 软件架构

patch - openeuler-21.03版本的内核实现信号监测的补丁程序

test_demo - 
    test_siguser: 测试信号监测的demo，先运行recv,后运行sender，sender向recver发送的信号可以在用户态日志中观察到
    test_sigint: 其他测试demo，其他信号可以自行测试

user_proc - 
    user_sig.c: 系统启动完毕设置在后台运行的用户态程序，用于接收内核发送的信号数据; 
    instruct.c: 通过运行该程序并输入指定信号，以实现内核信号监测的过滤功能

    instruct需要输入：
        1、user_sig的运行进程号
        2、10，通过SIGUSER1向user_sig发送信号过滤请求，user_sig再通过netlink协议通知内核屏蔽监测信号
        3、屏蔽或释放的信号值，e.g: 17表示屏蔽17号信号的监测，-17表示解除对17号信号值的屏蔽

#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
