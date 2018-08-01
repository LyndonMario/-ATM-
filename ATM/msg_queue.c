#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "status.h"

int get_msg(key_t key)
{
	int msgid = msgget(key,0644|IPC_CREAT);

	if(ERROR == msgid)
	{
		perror("msgget"),_exit(ERROR);
	}

	return msgid;
}

int get_snd_msg(void)
{
	return get_msg(ftok("ctos",56));
}

int get_rcv_msg(void)
{
	return get_msg(ftok("stoc",36));
}
