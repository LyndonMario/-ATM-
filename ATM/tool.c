#include "msg_queue.h"
#include <strings.h>
#include "tool.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "get_keyboard.h"





//初始化发送消息缓冲区变量
void init_msgbuf_snd(msgbuf_snd *buf_snd)
{
	buf_snd->mtype = 0;
	bzero(buf_snd->data.name,20);
	bzero(buf_snd->data.id,20);
	bzero(buf_snd->data.phone,20);
	bzero(buf_snd->data.number,20);
	bzero(buf_snd->data.password,20);
	buf_snd->data.money = 0;
	buf_snd->data.state = 0;
	buf_snd->pid = 0;

}


//初始化接收消息缓冲区变量
void init_msgbuf_rcv(msgbuf_rcv *buf_rcv)
{
	buf_rcv->mtype = 0;
	bzero(buf_rcv->mtext,100);
}


//清屏
void clear_scr(void)
{
	system("clear");
}

//清理输入缓冲区
void clear_stdin(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}

//获取字符串
char* get_str(char* str,size_t size)
{
	clear_stdin();

	fgets(str,size,stdin);
	size_t len = strlen(str);

	if(str[len-1] == '\n') 
	{
		str[len-1] = '\0';
	}

	clear_stdin();
}

//获取密码
char* get_pd(char* pd,size_t size)
{
	clear_stdin();

	int index = 0;
	while(index < size-1)
	{
		char val = get_keyboard();

		if(KEY_ENTER == val) break;
		
		if(KEY_BACKSPACE == val)
		{
			if(0 >= index) continue;

			printf("\b \b");
			index--;
		}
		
		pd[index++] = val;
		printf("*");
	}

	clear_stdin();	
	pd[index] = '\0';
	printf("\n");

	return pd;
}

//获取指令
char get_cmd(char start,char end)
{
	clear_stdin();

	printf("请输入指令:");
	while(true)
	{
		char val = get_keyboard();
		if(val >= start && val <= end)
		{
			printf("%c",val);
			getchar();
			return val;
		}
	}
}

// 初始化id文件
int init_id(const char* path,int id)
{
	if(OK == access(path,F_OK))
	{
		return 0;
	}

	int fd = open(path,O_WRONLY | O_CREAT | O_TRUNC,0644);
	if(ERROR == fd)
	{
		perror("open:init_id");
		return -1;
	}

	if(0 >= write(fd,&id,sizeof(id)))
	{
		perror("error");
		return -1;
	}

	close(fd);

	return 0;
}

//获取ID
int get_id(const char* path)
{
	int id = 0;
	
	int fd = open(path,O_RDWR);
	if(ERROR == fd)
	{
		perror("open:get_id");
		return -1;
	}

	if(0 >= read(fd,&id,sizeof(id)))
	{
		perror("read");
		return -1;
	}

	id++;	

	lseek(fd,SEEK_SET,0);

	if(0 >= write(fd,&id,sizeof id))
	{
		perror("write");
		return -1;
	}

	close(fd);
	
	return id - 1;
}

//输入任意键继续
void anykey_continue(void)
{
	printf("请输入任意键继续...");
	get_keyboard();
	printf("\n");
}

//显示提示信息
void show_msg(char* msg,float sec)
{
	printf("\33[;34m");
	printf("*** %s *** \n",msg);
	usleep(sec*1000000);
	printf("\33[0m");
}



