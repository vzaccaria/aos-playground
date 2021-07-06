mkfs.vfat -F 32 /dev/loop0p2
mkfs.ext4 /dev/loop0p3
mkdir -p /efi
mount /dev/loop0p2 /efi
mkdir -p /efi/boot
cp uefi-kernel/bzImage.efi /efi/boot/bootx64.efi
cp uefi-kernel/initrd.img /efi/boot
umount /efi
losetup -D

