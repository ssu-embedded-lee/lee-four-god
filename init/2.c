#include <stdio.h>
int main(void)
{
	system("cp arch/arm/boot/dts/overlays/*.dtb* /mnt/fat32/overlays/");
	return 0;
}
