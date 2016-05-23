#include <stdio.h>
int main(void)
{
	system("export PATH=$PATH:$HOME/raspberry/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin");
	system("arm-linux-gnueabihf-gcc");
	system("KERNEL=kernel7");
	system("make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig");
	system("make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j5");
	system("mount /dev/sdb1 /mnt/fat32");
	system("mount /dev/sdb2 /mnt/ext4");
	system("make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=/mnt/ext4 modules_install");
	system("./scripts/mkknlimg arch/arm/boot/zImage /mnt/fat32/$KERNEL.img");
	system("cp arch/arm/boot/dts/*.dtb /mnt/fat32/");
	return 0;
}
