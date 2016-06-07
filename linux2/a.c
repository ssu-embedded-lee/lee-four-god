#include<stdio.h>

int main()
{
	system("cp kernel/structGenie.c ~/raspberry/linux/kernel/structGenie.c");
	system("cp init/main.c ~/raspberry/linux/init/main.c");
	system("cp include/linux/syscalls.h ~/raspberry/linux/include/linux/syscalls.h");
	system("cp arch/arm/include/uapi/asm/unistd.h ~/raspberry/linux/arch/arm/include/uapi/asm/unistd.h");
	system("cp arch/arm/kernel/calls.S ~/raspberry/linux/arch/arm/kernel/calls.S");
}
