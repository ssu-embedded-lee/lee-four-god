#include  <stdio.h>
int main(void)
{
	system("umount /mnt/fat32");
	system("umount /mnt/ext4");
	return 0;
}
