#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkageint int sys_geniesyscall6()
{
	printk("sys_geniesyscall6()\n");
	return 1;
}
