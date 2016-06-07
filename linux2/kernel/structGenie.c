#ifndef STRUCT_GENIE
#define STRUCT_GENIE


#define SAVEFILE		"/home/pi/genieStruct.save"
#define PIDFILE			"/home/pi/geniePid.save"

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
#define EMPTY_FILE		17
#define NO_FILE			18

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fcntl.h>
#include<linux/slab.h>
#include<asm/uaccess.h>
#include<../lib/string.c>
#include<linux/vmalloc.h>
#include<linux/syscalls.h>
#include<linux/fs.h>
#include<linux/file.h>

struct device_Genie	{
	int dev_state;
	char dev_name[TOKEN_LEN];
	char dev_token[NUM_TOKEN][TOKEN_LEN];
	struct device_Genie *next;
};

int init_Genie(void);	//지니 초기화
void printAll(void);	//현재 상태 전부출력
//asmlinkage long sys_genieSave(void);	//지니 세이브
//asmlinkage long sys_genieLoad(void);	//지니 로드
void initStruct(struct device_Genie *node,const char *name);	//노드를 초기화
//asmlinkage long sys_genieSetDevice(const char *name_dev);		//디바이스명을 넣으면 해당 디바이스를 리스트에 넣어줌. 성공시 1 리턴
//asmlinkage long sys_genieSetToken(const char *name_dev, const char *name_token);	//이미 존재하는 디바이스명과 명령어를 주면 명령어를 리스트에 넣어줌. 성공시 1 리턴
//asmlinkage long sys_genieCheckCommand(const char *command);	//입력받은 커맨드가 유효하면 1 리턴
//asmlinkage int sys_geniesyscall6(char *unused);
//asmlinkage int sys_geniesyscall7(char *unused);
//asmlinkage int sys_geniesyscall8(char *unused);
int errorHandler(int errorNumber);	//에러시 에러처리 후 -1리턴


struct device_Genie *genie;			//리스트 헤드역할을 하는 전역변수
mm_segment_t oldfs;					//errorHandler에서 사용하기 위해 전역으로 선언함
extern pid_t pid_chrome;


int init_Genie()
{
	return 0;
}

void printAll()
{
	int i;
	struct device_Genie *cursor;
	if(genie == NULL) return;
	cursor = genie;
	while(cursor != NULL)	{
		printk("\n");
		if(cursor->dev_state == DEVICE_OFF) printk("State : DEVICE_OFF\n");
		else if(cursor->dev_state == DEVICE_ON) printk("State : DEVICE_ON\n");
		else if(cursor->dev_state == DEVICE_EXIST) printk("State : DEVICE_EXIST\n");
		else	printk("Unknown State\n");
		printk("Name : %s\n",cursor->dev_name);
		for(i=0;i<NUM_TOKEN;i++)
			printk("token %d : %s\n",i,cursor->dev_token[i]);
		cursor = cursor->next;
	}
}

asmlinkage long sys_genieSave()
{
	int i,fd;
	struct device_Genie *cursor;
	struct file *file;
	loff_t pos=0;

	oldfs = get_fs();
	set_fs(get_ds());

	fd = sys_open(SAVEFILE,O_RDWR|O_CREAT|O_TRUNC,0600);
	file = fget(fd);
	if(genie != NULL)	{
		cursor = genie;
		while(cursor != NULL)	{			//다음 자료구조는 \n으로, 명령어리스트 시작시 &으로, 다음인덱스는 |로 구분 
			vfs_write(file,"\n",1,&pos);
			vfs_write(file,&(cursor->dev_state),sizeof(int),&pos);
			vfs_write(file,cursor->dev_name,strlen(cursor->dev_name),&pos);
			vfs_write(file,"&",1,&pos);
			for(i=0 ;i<NUM_TOKEN ; i++)	if(cursor->dev_token[i][0] != '\0')	{
					vfs_write(file,"|",1,&pos);
					vfs_write(file,cursor->dev_token[i],strlen(cursor->dev_token[i]),&pos);		
			}
			cursor = cursor->next;
		}
	}
	vfs_write(file,"#",1,&pos);			//파일의 끝을 #으로 표시.
	fput(file);
	sys_close(fd);
	set_fs(oldfs);
	return 1;
}

asmlinkage long sys_genieLoad()
{
	int fd,offset,index=0;
	struct device_Genie *cursor;
	char *chCursor;
	char ch;
	struct file *file;
	loff_t pos = 0;

	oldfs = get_fs();
	set_fs(get_ds());
	if((fd = sys_open(SAVEFILE,O_RDONLY,0600)) >= 0)	{
		file = fget(fd);
		vfs_read(file,&ch,1,&pos);
		if(ch != '\n') return errorHandler(EMPTY_FILE);
		genie = (struct device_Genie *)vmalloc(sizeof(struct device_Genie));
		initStruct(genie,"");
		cursor = genie;
		vfs_read(file,&(genie->dev_state),sizeof(int),&pos);
		chCursor = cursor->dev_name;
		offset = 0;
		while(ch != '#')	{
			vfs_read(file,&ch,1,&pos);
			switch(ch)
			{
				case '\n' :
					cursor->next = (struct device_Genie *)vmalloc(sizeof(struct device_Genie));
					cursor = cursor->next;
					initStruct(cursor,"");
					vfs_read(file,&(cursor->dev_state),sizeof(int),&pos);
					chCursor = cursor->dev_name;
					offset = 0;
					break;
				case '&' :
					index = 0;
					chCursor[offset] = '\0';
					chCursor = cursor->dev_token[index];
					offset = 0;
					break;
				case '|' :
					chCursor[offset] = '\0';
					chCursor = cursor->dev_token[index++];
					offset = 0;
					break;
				case '#' :
					chCursor[offset] = '\0';
					break;
				default :
					chCursor[offset++] = ch;
					break;
			}
		}	
		sys_close(fd);
	}
	else return errorHandler(NO_FILE);
	set_fs(oldfs);
	return 1;
}

void initStruct(struct device_Genie *node,const char *name)
{
	int i,j;
	//node = (struct device_Genie *)vmalloc( sizeof(struct device_Genie *));
	node->dev_state = DEVICE_OFF;
	strcpy(node->dev_name,name);
	for(i=0;i<NUM_TOKEN;i++) for(j=0;j<TOKEN_LEN;j++) node->dev_token[i][j] = '\0';
	strcpy(node->dev_token[0],"TurnOn");
	strcpy(node->dev_token[1],"TurnOff");
	node->next = NULL;
}
asmlinkage long sys_genieSetDevice(const char *name_dev)		//디바이스명을 넣으면 해당 디바이스를 리스트에 넣어줌. 성공시 1 리턴
{
    struct device_Genie *cursor = genie;
	if(genie == NULL)
	{
		genie = (struct device_Genie *)vmalloc( sizeof(struct device_Genie));
		initStruct(genie,name_dev);
		return 1;
	}
    while(cursor->next != NULL)
    {
		if(!strcmp(cursor->next->dev_name,name_dev)) return errorHandler(DEVICE_EXIST);
		cursor = cursor->next;
    }
    cursor->next = (struct device_Genie *)vmalloc( sizeof(struct device_Genie) );
	initStruct(cursor->next,name_dev);
	sys_genieSave();
    return 1;
}

asmlinkage long sys_genieSetToken(const char *name_dev, const char *name_token)	//이미 존재하는 디바이스명과 명령어를 주면 명령어를 리스트에 넣어줌. 성공시 1 리턴
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
	sys_genieSave();
    return errorHandler(NO_DEVICE);
}

asmlinkage long sys_genieCheckCommand(const char *command)	//입력받은 커맨드가 유효하면 1 리턴
{
	int i,j,k=0;
	int len = strlen(command);
	int flag;
	struct device_Genie *cursor = genie;
	//printk("input : %s, length : %d\n",command,len);
	//printk("%s %s %s\n",genie->next->dev_name,genie->next->dev_token[0],genie->next->dev_token[1]);
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
	return errorHandler(NO_COMMAND);
}

int errorHandler(int errorNumber)	//에러시 에러처리 후 -1리턴
{
    switch (errorNumber)
    {
	case NO_DEVICE :
	    printk("device not exist.\n");
	    break;
	case DEVICE_EXIST :
	    printk("device already exist.\n");
	    break;
	case TOKEN_EXIST :
		printk("token already exist.\n");
		break;
	case NO_COMMAND :
		printk("command not found.\n");
		break;
	case NO_FILE :
		printk("savefile not exist.\n");
		set_fs(oldfs);
		break;
	case EMPTY_FILE :
		printk("savefile is empty.\n");
		set_fs(oldfs);
		break;
	default :
	    printk("Unknown Error.\n");
	    break;
    }
    return -1;
}

asmlinkage int sys_geniePid(pid_t inputPid)//커널에 자신의 pid를 알려주기위한 함수. pid를 찾아서 인자로 넣어야함
{
	pid_chrome = inputPid;
	return 1;
}

asmlinkage int sys_genieState(const char *name_dev, int input_state)	//기기의 state를 변경하거나 얻는 함수.
{
	int i;
    struct device_Genie *cursor = genie;
    while(cursor != NULL)	{
		if(!strcmp(cursor->dev_name, name_dev)) {
			if(input_state < 0) return cursor->dev_state;
			else cursor->dev_state = input_state;
			return 1;
		}
		cursor = cursor->next;
    }
	sys_genieSave();
    return errorHandler(NO_DEVICE);
}

asmlinkage int sys_geniesyscall8(char *unused)
{
	return 1;
}


#endif
