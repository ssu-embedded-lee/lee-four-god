/* FILE : fnd_driver.c */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/version.h>

#define FND_MAJOR 261
#define FND_NAME "fpga_fnd"
#define FND_ADDRESS 0x07000004
#define DEMO_ADDRESS 0x07000300
#define UON 0x00
#define UOFF 0x01

static int fnd_port_usage = 0;
static unsigned char *fnd_addr;
static unsigned char *demo_addr;

int __init find_init(void);
void __exit fnd_exit(void);
int fnd_open(struct inode *minode, struct file *mfile);
int fnd_release(struct inode *minode, struct file *mfile);
ssize_t fnd_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
ssize_t fnd_read(struct file *inode, char *gdata, size_t length, loff_t *off_what);

struct file_operations fnd_fops =
{
	//.owner = THIS_MODULE,
	.open = fnd_open,
	.write = fnd_write,
	.read = fnd_read,
	.release = fnd_release,
};

int fnd_open(struct inode *minode, struct file *mfile){
	if(fnd_port_usage != 0)
		return -EBUSY;
	fnd_port_usage = 1;
	return 0;
}

int fnd_release(struct inode *minode, struct file *mfile){
	fnd_port_usage = 0;
	return 0;
}

ssize_t fnd_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what){
	int i;
	unsigned char value[4];
	const char *tmp = gdata;
	if(copy_from_user(&value, tmp, 4))
		return -EFAULT;
	for(i=0;i<length;i++)
		outb(value[i],(unsigned int)fnd_addr+i);
	return length;
}

ssize_t fnd_read(struct file *inode, const char *gdata, size_t length, loff_t *off_what){
	int i;
	unsigned char value[4];
	char *tmp = gdata;
	for(i=0;i<length;i++){
		value[i] = inb((unsigned int)fnd_addr+i);
	}
	if(copy_to_user(tmp,value,4))
		return -EFAULT;
	return length;
}

int __init find_init(void){
	int result;
	result = register_chrdev(FND_MAJOR, FND_NAME, &fnd_fops);
	if(result < 0){
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	fnd_addr = ioremap(FND_ADDRESS, 0x4);
	demo_addr = ioremap(DEMO_ADDRESS, 0x1);
	outb(UON,(unsigned int)demo_addr);
	printk("init module, %s major number : %d\n",FND_NAME, FND_MAJOR);
	return 0;
}

void __exit fnd_exit(void){
	iounmap(fnd_addr);
	iounmap(demo_addr);
	unregister_chrdev(FND_MAJOR, FND_NAME);
}

module_init(find_init);
module_exit(fnd_exit);

MODULE_LICNESE("GPL");
MODULE_AUTHOR("OSLAB");
