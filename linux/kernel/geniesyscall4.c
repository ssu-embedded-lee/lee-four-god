#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall4(char *temp)
{
	printk("sys_geniesyscall4()\n");
	return 1;
}
