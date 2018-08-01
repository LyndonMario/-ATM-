#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "tool.h"
#include "account.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>


int log_state = 0;
char login_number[20] = {};

static int msg_snd_id;
static int msg_rcv_id;

//创建发送消息缓冲区，接受消息缓冲区
static msgbuf_snd buf_snd;
static msgbuf_rcv buf_rcv;


void msg_send(void)
{
	if(ERROR == msgsnd(msg_snd_id,&buf_snd,sizeof(buf_snd)-4,0))
	{
		perror("msgsnd");
		_exit(-1);
	}
}


void msg_recv(void)
{
	if(ERROR == msgrcv(msg_rcv_id,&buf_rcv,sizeof(buf_rcv)-4,buf_rcv.mtype,0))
	{
		perror("msgrcv");
		_exit(-1);
	}

	show_msg(buf_rcv.mtext,1.5);
}





void open()
{
	
	printf("请输入姓名：");
	scanf("%s",buf_snd.data.name);
					

	printf("请输入身份证号：");
	scanf("%s",buf_snd.data.id);

				
	printf("请输入电话：");
	scanf("%s",buf_snd.data.phone);


	printf("请输入密码：");
	get_pd(buf_snd.data.password,sizeof(buf_snd.data.password));
				
				
	//设置发送消息类型,pid
	buf_snd.mtype = M_OPEN;
	buf_snd.pid = getpid();
				
}


void destroy(void)
{
	printf("请输入要注销的帐号：");
	get_str(buf_snd.data.number,sizeof(buf_snd.data.number));

	printf("请输入要注销的用户名：");
	get_str(buf_snd.data.name,sizeof(buf_snd.data.name));


	printf("请输入密码：");
	get_pd(buf_snd.data.password,sizeof(buf_snd.data.password));

	buf_snd.mtype = M_DESTROY;

}


void login(void)
{
	printf("请输入帐号：");
	get_str(buf_snd.data.number,sizeof(buf_snd.data.number));

	
	printf("请输入密码：");
	get_pd(buf_snd.data.password,sizeof(buf_snd.data.password));
	buf_snd.mtype  = M_LOGIN;
	msg_send();
	if(ERROR == msgrcv(msg_rcv_id,&buf_rcv,sizeof(buf_rcv)-4,getpid(),0))
	{
		perror("msgrcv");
		_exit(-1);
	}
	if('s' == *buf_rcv.mtext)
	{
		log_state = 1;
		strcpy(login_number,buf_snd.data.number);
	}
	show_msg(buf_rcv.mtext+1,1);

}

//向服务器发送消息 接受系统发送的服务器pid，向服务器进程发送SIGINT信号使其关闭服务器进程，然后退出此进程
void exit_client(void)
{
	char str[100] = "系统正在退出...";
	show_msg(str,1);
	buf_snd.mtype  = M_EXIT;
	msg_send();
	msg_recv();
	kill(atoi(buf_rcv.mtext),SIGINT);
	exit(0);
}


void unlock(void)
{
	printf("请输入帐号：");
	get_str(buf_snd.data.number,sizeof(buf_snd.data.number));
	
	
	printf("请输入密码：");
	get_pd(buf_snd.data.password,sizeof(buf_snd.data.password));
	
	
	printf("请输入身份证号：");
	get_pd(buf_snd.data.id,sizeof(buf_snd.data.id));


	buf_snd.mtype  = M_UNLOCK;
	msg_send();


	if(ERROR == msgrcv(msg_rcv_id,&buf_rcv,sizeof(buf_rcv)-4,getpid(),0))
	{
		perror("msgrcv");
		_exit(-1);
	}


	show_msg(buf_rcv.mtext+1,1);


}

//登陆状态下查询某个账户的信息
void query(void)
{
	strcpy(buf_snd.data.number,login_number);
	buf_snd.mtype = M_QUERY;
	
	msg_send();
	if(ERROR == msgrcv(msg_rcv_id,&buf_rcv,sizeof(buf_rcv)-4,buf_rcv.mtype,0))
	{
		perror("msgrcv");
		_exit(-1);
	}

	show_msg(buf_rcv.mtext,1.5);
	
	anykey_continue();

}


//转账功能，接收目的人的帐号，姓名，转账金额分别存放在buf_snd.data的password,name,money中

void transf(void)
{
	strcpy(buf_snd.data.number,login_number);

	printf("请输入转账目的人帐号：");
	get_str(buf_snd.data.password,sizeof(buf_snd.data.password));
	
	
	printf("请输入转账目的人姓名：");
	get_str(buf_snd.data.name,sizeof(buf_snd.data.name));
	
	
	printf("请输入转帐金额：");
	scanf("%d",&buf_snd.data.money);


	buf_snd.mtype  = M_TRANSF;

	
}


void take(void)
{
	strcpy(buf_snd.data.number,login_number);
	
	printf("请输入取出金额：");
	scanf("%d",&buf_snd.data.money);
	buf_snd.mtype  = M_TAKE;
}


void save(void)
{
	strcpy(buf_snd.data.number,login_number);
	
	printf("请输入存入金额：");
	scanf("%d",&buf_snd.data.money);
	buf_snd.mtype  = M_SAVE;
}


void modify(void)
{
	strcpy(buf_snd.data.number,login_number);

	while(1)
	{
		printf("请输入原密码：");
		get_pd(buf_snd.data.password,sizeof(buf_snd.data.password));
	
	
		printf("请输入新密码：");
		get_pd(buf_snd.data.name,sizeof(buf_snd.data.name));
	
		
		char tmp[20] = {};
		printf("请再次输入新密码：");
		get_pd(tmp,sizeof(tmp));

		if(OK == strcmp(buf_snd.data.name,tmp))
		{
			break;
		}
		printf("两次输入的新密码不一致，请重新输入...\n");

	}

	buf_snd.mtype  = M_MODIFY;

}


void logout(void)
{
	log_state = 0;
	bzero(login_number,20);
	
	char str[50] = "您已成功退出...";
	show_msg(str,1.5);
}


void show_menu1(void)
{
	system("clear");
		printf("欢迎来到杭州指针银行\n");
		printf("1.开户\n");
		printf("2.销户\n");
		printf("3.登陆\n");
		printf("4.关机\n");
		printf("5.解锁\n");
}


void show_menu2(void)
{
	system("clear");
		printf("欢迎来到杭州指针银行\n");
		printf("1.查询账户余额\n");
		printf("2.转帐\n");
		printf("3.取钱\n");
		printf("4.存钱\n");
		printf("5.修改密码\n");
		printf("6.退出登陆\n");

}





int main()
{
	msg_snd_id = get_snd_msg();
	msg_rcv_id = get_rcv_msg();

	buf_snd.pid = buf_rcv.mtype = getpid();

	while(true)
	{
		if(!log_state)
		{
			show_menu1();
			switch(get_cmd('1','5') - '0')
			{
				case M_OPEN: open(); break;
				case M_DESTROY:destroy(); break;
				case M_LOGIN:login();continue;
				case M_EXIT:exit_client();
				case M_UNLOCK:unlock();continue;
			}
			msg_send();
			msg_recv();
		}
		else
		{
			show_menu2();
			switch(get_cmd('1','6') - '0'+5)
			{
				
				case M_QUERY: query();continue;
				case M_TRANSF:transf();break;
				case M_TAKE:take(); break;
				case M_SAVE:save();break;
				case M_MODIFY:modify();break;
				case M_LOGOUT:logout();continue;
				
			}
			msg_send();
			msg_recv();
			
		}	
	//	bzero(&buf_snd,sizeof(buf_snd));
	//	bzero(&buf_rcv,sizeof(buf_rcv));
	}
}









