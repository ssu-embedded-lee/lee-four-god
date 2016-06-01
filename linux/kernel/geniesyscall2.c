#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall2(char *temp)
{
	printk("sys_geniesyscall2()\n");
	do_execve(getname("/home/pi/ex2"),NULL,NULL);
	return 1;
}
