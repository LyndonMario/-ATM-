#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

//此函数能立即从键盘不回显的接收数据
int32_t get_keyboard(void)
{
	//接收系统调用的执行结果
	int32_t ret = 0;
	//存储终端设备的配置信息
	struct termios old;
	
	//通过系统调用获取终端的配置信息
	ret=tcgetattr(STDIN_FILENO,&old);
	if(0 > ret)
	{
		perror("tcgetattr");
		return -1;
	}
	
	//初始化新的终端配置信息
	struct termios new = old;
	//取消回显并立即获取
	new.c_lflag &= ~(ICANON|ECHO);
	
	//设置新的终端配置信息
	ret= tcsetattr(STDIN_FILENO,TCSANOW,&new);
	if(0 > ret)
	{
		perror("tcsetattr");
		return -2;
	}
	
	//在新的模式下从终端获取数据
	int32_t key_value = 0;
	do{
		key_value += getchar();
	}while(stdin->_IO_read_end - stdin->_IO_read_ptr);
	
	//还原终端的配置信息
	ret = tcsetattr(STDIN_FILENO,TCSANOW,&old);
	if(0 > ret)
	{
		perror("tcsetattr");
		return -3;
	}

	//返回获取到的数据
	return key_value;
}
