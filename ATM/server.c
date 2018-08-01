#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "account.h"
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include "msg_queue.h"
#include "signal.h"
#include "tool.h"

static int msg_snd_id;
static int msg_rcv_id;
static pid_t sub_pid[9];
msgbuf_snd buf_snd;
msgbuf_rcv buf_rcv;




void sigint_close(int signum)
{
	msgctl(msg_snd_id,IPC_RMID,NULL);
	msgctl(msg_rcv_id,IPC_RMID,NULL);

	int i;
	for(i=0;i < 9;i++)
	{
		kill(sub_pid[i],SIGINT);
	}
	exit(0);

}



int main()
{
	signal(SIGINT,sigint_close);
	
	init_id(ID_PATH,20180727);

	msg_snd_id = get_snd_msg();
	msg_rcv_id = get_rcv_msg();

	sub_pid[0] = vfork();

	if(0 == sub_pid[0])
	{
		execl("open","open",NULL);
	}

	
	sub_pid[1] = vfork();
	if(0 == sub_pid[1])
	{
		execl("destroy","destroy",NULL);
	}

	
	
	sub_pid[2] = vfork();
	if(0 == sub_pid[2])
	{
		execl("login","login",NULL);
	}
	

	sub_pid[3] = vfork();
	if(0 == sub_pid[3])
	{
		execl("unlock","unlock",NULL);
	}

	sub_pid[4] = vfork();
	if(0 == sub_pid[4])
	{
		execl("query","query",NULL);
	}

	sub_pid[5] = vfork();
	if(0 == sub_pid[5])
	{
		execl("transf","transf",NULL);
	}

	sub_pid[6] = vfork();
	if(0 == sub_pid[6])
	{
		execl("take","take",NULL);
	}

	sub_pid[7] = vfork();
	if(0 == sub_pid[7])
	{
		execl("save","save",NULL);
	}

	sub_pid[8] = vfork();
	if(0 == sub_pid[8])
	{
		execl("modify","modify",NULL);
	}


	//父进程用来等待接受退出命令
	while(true)
	{
		if(ERROR == msgrcv(msg_snd_id,&buf_snd,sizeof(buf_snd)-4,M_EXIT,0))
		{
			perror("msgrcv");
		}

		buf_rcv.mtype = buf_snd.pid;
		sprintf(buf_rcv.mtext,"%d",getpid());

		if(ERROR == msgsnd(msg_rcv_id,&buf_rcv,sizeof(buf_rcv.mtext),0))
		{
			perror("msgsnd");
		}

	}
	
	
	wait();
}







