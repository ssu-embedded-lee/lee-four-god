#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall1()
{
	printk("sys_geniesyscall1()\n");
	return 1;
}
