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



//返回消息的字符串中的第一个字符表示登陆成功('s')或失败('f')
void unlock_acc(void)
{
	char acc_path[255] = {};
	sprintf(acc_path,"%s%s",ACCOUNT_PATH,buf_snd.data.number);


	//察看是否存在这个文件，不存在则说明这个帐号不存在
	if(ERROR == access(acc_path,F_OK))
	{
		sprintf(buf_rcv.mtext,"f帐号不存在");
		return;
	}


	int fd = open(acc_path,O_RDWR);

	if(ERROR == fd)
	{
	
		perror("open");		
		sprintf(buf_rcv.mtext,"f大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
	
		return;
	}
	

	account acc = {};

	if(0 >= read(fd,&acc,sizeof(acc)))
	{
		perror("read");
		sprintf(buf_rcv.mtext,"f大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
		close(fd);
		return;
	}

	



	if(OK == strcmp(acc.password,buf_snd.data.password)&& OK == strcmp(acc.id,buf_snd.data.id))
	{
		acc.state = 0;
		sprintf(buf_rcv.mtext,"s您的帐号%s已经解锁完成...",acc.number);
		
		close(fd);

		fd = open(acc_path,O_RDWR | O_TRUNC);


		if(0 >= write(fd,&acc,sizeof(acc)))
		{
			perror("write");
			sprintf(buf_rcv.mtext,"f大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
			close(fd);
			return;
		}
		
		
		close(fd);
		return;
	}


	close(fd);


	sprintf(buf_rcv.mtext,"f身份证或密码错误，解锁失败...");

}





int main()
{
	msg_snd_id = get_snd_msg();
	msg_rcv_id = get_rcv_msg();

	while(true)
	{
		if(ERROR == msgrcv(msg_snd_id,&buf_snd,sizeof(buf_snd)-4,M_UNLOCK,0))
		{
			perror("msgrcv");
		}

		unlock_acc();

		buf_rcv.mtype = buf_snd.pid;

		if(ERROR == msgsnd(msg_rcv_id,&buf_rcv,sizeof(buf_rcv.mtext),0))
		{
			perror("msgsnd:unlock");
		}

	}

}
