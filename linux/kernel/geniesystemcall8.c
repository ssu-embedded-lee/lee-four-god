#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall8()
{
	printk("sys_geniesyscall8()\n");
	return 1;
}
