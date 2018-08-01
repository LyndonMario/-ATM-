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


void destroy_acc(void)
{
	char acc_path[255] = {};
	sprintf(acc_path,"%s%s",ACCOUNT_PATH,buf_snd.data.number);


	//察看是否存在这个文件，不存在则说明这个帐号不存在
	if(ERROR == access(acc_path,F_OK))
	{
		sprintf(buf_rcv.mtext,"帐号：%s不存在",buf_snd.data.number);
		return;
	}


	int fd = open(acc_path,O_RDONLY);

	if(ERROR == fd)
	{
	
		perror("open");		
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
	
		return;
	}
	

	account acc = {};

	if(0 >= read(fd,&acc,sizeof(acc)))
	{
		perror("read");
		sprintf(buf_rcv.mtext,"大师兄带着小姨子,携款逃跑了，正在追捕中，请稍后在试...");
		return;
	}



	if(OK != strcmp(acc.name,buf_snd.data.name)|| OK != strcmp(acc.password,buf_snd.data.password))
	{
		sprintf(buf_rcv.mtext,"帐号或密码错误...");
		return;
	}


	close(fd);

	if(ERROR == unlink(acc_path))
	{
		perror("unlink");
		sprintf(buf_rcv.mtext,"帐号或密码错误...");
		return;
	}

	sprintf(buf_rcv.mtext,"帐号：%s销户成功！",buf_snd.data.number);

}





int main()
{
	msg_snd_id = get_snd_msg();
	msg_rcv_id = get_rcv_msg();

	while(true)
	{
		if(ERROR == msgrcv(msg_snd_id,&buf_snd,sizeof(buf_snd)-4,M_DESTROY,0))
		{
			perror("msgrcv");
		}
		destroy_acc();

		buf_rcv.mtype = buf_snd.pid;

		if(ERROR == msgsnd(msg_rcv_id,&buf_rcv,sizeof(buf_rcv.mtext),0))
		{
			perror("msgsnd");
		}

	}

}
