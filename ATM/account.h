#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct account
{
	char name[20];
	char id[20];
	char phone[20];
	char number[20];
	char password[20];
	int  money;
	int state;
}account;



#endif  //ACCOUNT_H
