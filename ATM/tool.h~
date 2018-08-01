#ifndef TOOL_H
#define TOOL_H

#include "msg_queue.h"
#include "status.h"

#define ACCOUNT_PATH 	"account/"
#define ID_PATH		 	"account/id.dat"


//初始化发送消息缓冲区变量
void init_msgbuf_snd(msgbuf_snd *buf_snd);


//初始化接收消息缓冲区变量
void init_msgbuf_rcv(msgbuf_rcv *buf_rcv);


//清屏
void clear_scr(void);

//清理输入缓冲区
void clear_stdin(void);

//获取字符串
char* get_str(char* str,size_t size);

//获取密码
char* get_pd(char* pd,size_t size);

//获取指令
char get_cmd(char start,char end);

//初始化id
int init_id(const char* path,int id);

//获取ID
int get_id(const char* path);

//输入任意键继续
void anykey_continue(void);

//显示提示信息
void show_msg(char* msg, float sec);


#endif  //TOOL_H
