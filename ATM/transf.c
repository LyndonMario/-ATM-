#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <signal.h>
#include "tool.h"
#include "msg_queue.h"
#include <string.h>
#include <stdlib.h>


static int msg_snd_id;
static int msg_rcv_id;
msgbuf_snd buf_snd;
msgbuf_rcv buf_rcv;



void transf_acc(void)
{

	/*输入进来的信息格式为，当前帐号存放在buf_snd.data.number中，而目的人帐号存放在buf_snd.data.password，我这里其实是偷懒，取巧了，讲道理应该重新写一个发送数据结构体比较严谨，另外要转的金额存放在buf_snd.data.money中*/



/*先判断当前账户是否存在，有的钱是否够转。*/

	
	char acc_path_from[255] = {};
	sprintf(acc_path_from,"%s%s",ACCOUNT_PATH,buf_snd.data.number);


	//察看是否存在这个文件，不存在则说明这个帐号不存在
	if(ERROR == access(acc_path_from,F_OK))
	{
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
		return;
	}


	int fd_from = open(acc_path_from,O_RDWR);

	if(ERROR == fd_from)
	{
	
		perror("open");		
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
	
		return;
	}
	

	account acc_from = {};

	if(0 >= read(fd_from,&acc_from,sizeof(acc_from)))
	{
		perror("read");
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
		close(fd_from);
		return;
	}

	
	//当前账户余额不够
	if(buf_snd.data.money > acc_from.money)
	{
		sprintf(buf_rcv.mtext,"您的帐号：%s余额不够，转账失败...",acc_from.number);
		close(fd_from);
		return;
	}


//查找目的人是否存在，以及名字是否吻合,目的人帐号姓名存放在password,和name中
	char acc_path_to[255] = {};
	sprintf(acc_path_to,"%s%s",ACCOUNT_PATH,buf_snd.data.password);


	//察看是否存在这个文件，不存在则说明这个帐号不存在
	if(ERROR == access(acc_path_to,F_OK))
	{
		sprintf(buf_rcv.mtext,"目的人帐号不存在，请重试...");
		return;
	}


	int fd_to = open(acc_path_to,O_RDWR);

	if(ERROR == fd_to)
	{
	
		perror("open");		
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
	
		return;
	}
	

	account acc_to = {};

	if(0 >= read(fd_to,&acc_to,sizeof(acc_to)))
	{
		perror("read");
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
		close(fd_to);
		return;
	}

	if(OK != strcmp(acc_to.name,buf_snd.data.name))
	{
		sprintf(buf_rcv.mtext,"目的账户的帐号%s与账户人姓名不匹配，请重试...",buf_snd.data.password,buf_snd.data.name);
		
		close(fd_to);
		return;		
	}


//此时说明条件都符合，写入数据。


		fd_to = open(acc_path_to,O_RDWR | O_TRUNC);
		acc_to.money += buf_snd.data.money;

		if(0 >= write(fd_to,&acc_to,sizeof(acc_to)))
		{
			perror("write");
			sprintf(buf_rcv.mtext,"f大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
			close(fd_to);
			return;
		}

//写入当前账户信息
		fd_from = open(acc_path_from,O_RDWR | O_TRUNC);
		acc_from.money -= buf_snd.data.money;

		if(0 >= write(fd_from,&acc_from,sizeof(acc_from)))
		{
			perror("write");
			sprintf(buf_rcv.mtext,"f大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
			close(fd_from);
			return;
		}



	close(fd_from);
	close(fd_to);

	sprintf(buf_rcv.mtext,"转账成功，当前账户余额为%d...",acc_from.money);

}





int main()
{
	msg_snd_id = get_snd_msg();
	msg_rcv_id = get_rcv_msg();

	while(true)
	{
		if(ERROR == msgrcv(msg_snd_id,&buf_snd,sizeof(buf_snd)-4,M_TRANSF,0))
		{
			perror("msgrcv");
		}

		transf_acc();

		buf_rcv.mtype = buf_snd.pid;

		if(ERROR == msgsnd(msg_rcv_id,&buf_rcv,sizeof(buf_rcv.mtext),0))
		{
			perror("msgsnd:transf");
		}

	}

}
