#ifndef SERVER_H
#define SERVER_H


#include <sys/types.h>
#include <sys/ipc.h>
#include "account.h"


//服务器处理主函数
int server(key_t key,account data[100]);





#endif  //SERVER_H
