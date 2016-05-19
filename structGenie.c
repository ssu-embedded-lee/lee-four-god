#ifndef STRUCT_GENIE
#define STRUCT_GENIE

#define NUM_DEVICE		10
#define NUM_TOKEN		10
#define TOKEN_LEN		30
#define NO_DEVICE		10
#define NO_COMMAND		11
#define NO_TOKEN		12
#define DEVICE_OFF		13
#define DEVICE_ON		14
#define DEVICE_EXIST	15
#define TOKEN_EXIST		16

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

struct device_Genie	{
	int dev_state;
	char dev_name[TOKEN_LEN];
	char dev_token[NUM_TOKEN][TOKEN_LEN];
	struct device_Genie *next;
};

struct device_Genie *genie;			//리스트 헤드역할을 하는 전역변수

int init_Genie()
{
	;
}

void printAll()
{
	int i;
	if(genie == NULL) return;
	struct device_Genie *cursor = genie;
	while(cursor != NULL)	{
		printf("State : %d\n",cursor->dev_state);
		printf("Name : %s\n",cursor->dev_name);
		for(i=0;i<NUM_TOKEN;i++)
			printf("token %d : %s\n",i,cursor->dev_token[i]);
		cursor = cursor->next;
	}
}

void saveGenie()
{
	int i,fd;
	char ch = '\n';
	struct device_Genie *cursor;
	mkdir("/Genie",0755);
	chdir("/Genie");
	printAll();
	fd = creat("struct",0644);
	if(genie != NULL)	{
		cursor = genie;
		while(cursor != NULL)	{			//다음 자료구조는 \n으로, 다음 인스턴스는 &으로, 다음인덱스는 |로 구분 
			write(fd,&ch,1);
			write(fd,cursor->dev_state,sizeof(int));
			write(fd,"&",1);
			write(fd,cursor->dev_name,strlen(cursor->dev_name));
			for(i=0 ;i<NUM_TOKEN ; i++)	if(cursor->dev_token[i][0] != '\0')	{
					write(fd,"|",1);
					write(fd,cursor->dev_token[i],strlen(cursor->dev_token[i]) );		
			}
			cursor = cursor->next;
		}
	}
	close(fd);
}
/*
void loadGenie()
{
	int i,j,fd;
	struct device_Genie *cursor;
	char ch;
	if (access("/Genie/struct"))	{
		fd = open("/Genie/struct");
		while(1)	{
			read(fd,&ch,1);
			if(ch != '\n') break;
			genie = (struct device_Genie *)malloc(sizeof(device_Genie *));
			cursor = genie;
			while(1)	{
					
				if(ch == '\n') 
			}
	}
}
*/
void initStruct(struct device_Genie *node,const char *name)
{
	int i;
	//node = (struct device_Genie *)malloc( sizeof(struct device_Genie *));
	node->dev_state = DEVICE_OFF;
	strcpy(node->dev_name,name);
	strcpy(node->dev_token[0],"TurnOn");
	strcpy(node->dev_token[1],"TurnOff");
	for(i=2;i<NUM_TOKEN;i++) node->dev_token[i][0] = '\0';
	node->next = NULL;
}
int setDevice(const char *name_dev)		//디바이스명을 넣으면 해당 디바이스를 리스트에 넣어줌. 성공시 1 리턴
{
    struct device_Genie *cursor = genie;
	if(genie == NULL)
	{
		genie = (struct device_Genie *)malloc( sizeof(struct device_Genie));
		initStruct(genie,name_dev);
		return 1;
	}
    while(cursor->next != NULL)
    {
		if(!strcmp(cursor->next->dev_name,name_dev)) return errorHandler(DEVICE_EXIST);
		cursor = cursor->next;
    }
    cursor->next = (struct device_Genie *)malloc( sizeof(struct device_Genie) );
	initStruct(cursor->next,name_dev);
    return 1;
}

int setToken(const char *name_dev, const char *name_token)	//이미 존재하는 디바이스명과 명령어를 주면 명령어를 리스트에 넣어줌. 성공시 1 리턴
{
    int i;
    struct device_Genie *cursor = genie;
    while(cursor != NULL)	{
	if(!strcmp(cursor->dev_name, name_dev))	{
		for(i=0;i<NUM_TOKEN;i++)	{
			if(!strcmp(cursor->dev_token[i],name_token)) return errorHandler(TOKEN_EXIST);
			if(strlen(cursor->dev_token[i]) == 0)	{
				strcpy(cursor->dev_token[i],name_token);
				return 1;
			}
		}
	}
	cursor = cursor->next;
    }
    return errorHandler(NO_DEVICE);
}

int checkCommand(const char *command)	//입력받은 커맨드가 유효하면 1 리턴
{
	int i,j,k=0;
	int len = strlen(command);
	int flag;
	struct device_Genie *cursor = genie;
	printf("input : %s, length : %d\n",command,len);
	printf("%s %s %s\n",genie->next->dev_name,genie->next->dev_token[0],genie->next->dev_token[1]);
	while(cursor != NULL)	{
		flag = 0;
		for(j=0 ; j<len && cursor->dev_name[j] != '\0' ; j++)	{
			if(command[j] != cursor->dev_name[j]) { flag = 1; break; }
		}
		if(!flag)	{
			for(i=0;i<NUM_TOKEN;i++)	{
				while(k<NUM_TOKEN)	{
					if(len-j != strlen(cursor->dev_token[k]) );
					else if(!strcmp(cursor->dev_token[k],command+j)) return 1;
					k++;
				}
			}
		}
		cursor = cursor->next;
	}
	printf("finish\n");
	return errorHandler(NO_COMMAND);
}

int errorHandler(int errorNumber)	//에러시 에러처리 후 -1리턴
{
    switch (errorNumber)
    {
	case NO_DEVICE :
	    printf("wrong device name.\n");
	    break;
	case DEVICE_EXIST :
	    printf("device already exist.\n");
	    break;
	case TOKEN_EXIST :
		printf("token already eixst.\n");
		break;
	case NO_COMMAND :
		printf("command not found.\n");
		break;
	default :
	    printf("Unknown Error.\n");
	    break;
    }
    return -1;
}
#endif
