#include <stdio.h>
#include <string.h>
#include "structGenie.c"
#include "1.h"	//	genie_Struct

#define STATE_SIZE 6
#define COMMAND_SIZE 150

/*
 * "지니" 입력 받는 것
 * 명령어 입력 받는 것
 * 명령어 번호대로 출력
 */

int getGenie();
// "지니" 명령어 받을 때까지 무한루프

int getCommand();
// 맞는 명령어 받을 때까지 무한루프
// 짝수 - ON	홀수 - OFF

int getState(int);
// 명령어에 따른 상태 출력

int doCommand(int,int);
// 명령어에 따른 상태 수행

void showState();
// 사용자 편의를 위해 상태 출력

int main(void)
{
	int ret;
	int state;
	int command;
	if(loadGenie() < 0)	{
		setDevice("Computer");
		setDevice("AirConditioner");
		saveGenie();
	}
	printAll();
	return 0;
	while(1)
	{
		ret = 0;
		state = 1;
		command = 0;
getchar();
		ret=getGenie();
		getchar();	//개행문자 제거
		showState();

		ret=getCommand();

		printf("Command Num : %d\n",ret);

		state=getState(ret/2);

		command=doCommand(ret,state);
	}

	return 0;
}

int getGenie()
{
	printf("CALL GENIE!\n");
	char command[100];
	while(1)
	{
		scanf("%s",command);
		if(!strcmp(command,"genie"))
			return 1;
		printf("genie라 불러주십시오.\n");
	}
}

int getCommand()
{
	while(1)
	{
		printf("명령을 내려주십시오.\n");
		if(getString() < 0)
			printf("잘못된 명령입니다.\n");
	}
}

int getString()
{
	int i=0;
	char command[COMMAND_SIZE];
	char ch=getchar();
	while(ch != '\n')	{
		command[i++] = ch;
		ch = getchar();
	}
	command[i] = '\0';
	return checkCommand(command);
}

int getState(int ret)
{
	if(dev_state[ret][0]==1)
	{
		printf("OFF STATE\n");
		return 0;
	}
	else if(dev_state[ret][1]==1)
	{
		printf("ON STATE\n");
		return 1;
	}
	else
	{
		printf("NO DEVICE\n");
		return -1;
	}
}

int doCommand(int ret, int state)
{
	int i;//

	printf("state : %d\tret : %d\n",state,ret);
	if(state<0)
	{
		printf("NO DEVICE ERROR!\n");
		return -1;
	}
	else if((state==0&&ret%2==0) || (state==1&&ret%2))
	{
		dev_state[ret/2][ret%2]=0;
		dev_state[ret/2][(ret+1)%2]=1;
	/*	if(state==0)
		{
			
		}
		else
		{
		}*/
		printf("명령을 수행했습니다\n");
		showState();
		return 1;
	}
	else
	{
		printf("명령을 수행할 수 있는 상태가 아닙니다.\n");
		return 0;
	}
}

void showState()
{
	int i;
	for(i=0;i<STATE_SIZE;i++)//
	{
		printf("%d ",dev_state[i/3][(i%3)]);
		if(i%3==2)
			printf("\n");
	}
	printf("\n");
}
