#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall7()
{
	printk("sys_geniesyscall7()\n");
	return 1;
}
