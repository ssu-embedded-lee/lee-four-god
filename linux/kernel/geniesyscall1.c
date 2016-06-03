#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_geniesyscall1(char *temp)
{
	printk("sys_geniesyscall1()\n");
	do_execve(getname("/home/pi/ex1"),NULL,NULL);
	return 1;
}
