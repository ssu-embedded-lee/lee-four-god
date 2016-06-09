#include<stdio.h>
#include<linux/unistd.h>
#include<unistd.h>
#include<fcntl.h>

#define save()					syscall(391,1,NULL)
#define load()					syscall(392,1,NULL)
#define autoload()				syscall(392,-1,NULL)
#define checkCommand(command)			syscall(393,command)
#define setDevice(dev_name)			syscall(394,dev_name)
#define setCommand(dev_name, dev_command)	syscall(395,dev_name,dev_command)
#define setPid(pid)				syscall(396,pid)
#define devState(dev_num,state)		syscall(397,dev_num,state)

#define NUM_TOKEN 10

int main(int argc, char *argv[])
{
	int ret;
	int tmp;
	int fd;
	char buf[100];
	char buf2[100];
	switch(argv[1][0])	
	{
		case '1' :							//save
			save();
			break;
		case '2' :							//load
			load();
			tmp = 0;
			while((ret = devState(tmp,-1)) >= 0) {
				///////
				//
				//	tmp'th dev's state : ret
				//	routine?
				//
				///////
				tmp++;
			}
			break;
		case '3' :							//Command Check
			checkCommand(argv[2]);
			break;
		case '4' :							//set Device
			setDevice(argv[2]);
			break;
		case '5' :							//set Command
			setCommand(argv[2],argv[3]);
			break;
		case '6' :							//setPid
			system("ps aux|awk '/chromium-browser/ {print $2}' > pid.txt");
			fd = open("./pid.txt",O_RDONLY);
			read(fd,buf,100);
			sscanf(buf,"%s %s %s",buf2,buf2,buf2);
			printf("%s\n",buf2);
			setPid(atoi(buf2));
			break;
		case '7' :							//Command execution
			ret = checkCommand(argv[2]);
			devState(ret / NUM_TOKEN ,ret % NUM_TOKEN);
			break;
		case '8' :
			autoload();
			while((ret = devState(tmp,-1)) >= 0) {
				///////
				//
				//	tmp'th dev's state : ret
				//	routine?
				//
				///////
				tmp++;
			}
			break;
		default :
			return -1;
	}
	return 1;
}
