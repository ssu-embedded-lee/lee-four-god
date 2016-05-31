#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall3(char *temp)
{
	printk("sys_geniesyscall3()\n");
	return 1;
}
