#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "account.h"
#include <sys/types.h>
#include <unistd.h>


#define	M_OPEN		1 	// 开户
#define M_DESTROY	2 	// 销户
#define M_LOGIN     3   //登陆
#define M_EXIT		4	// 退出
#define M_UNLOCK    5   //解锁
#define M_QUERY		6 	// 查询
#define M_TRANSF	7 	// 转账
#define M_TAKE		8 	// 取钱
#define M_SAVE		9 	// 存钱
#define M_MODIFY    10  //修改密码
#define M_LOGOUT    11  //退出登陆




typedef struct msgbuf_snd
{
	long mtype;
	account data;
	pid_t pid;
}msgbuf_snd;



typedef struct msgbuf_rcv
{
	long mtype;
	char mtext[100];
}msgbuf_rcv;




//获得发送消息队列标示符
int get_snd_msg(void);


//获得发送接受队列标示符
int get_rcv_msg(void);



#endif  //MSG_QUEUE_H
