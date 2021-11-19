USEUEFI=-drive if=pflash,format=raw,read-only,file=/usr/local/Cellar/qemu/5.1.0/share/qemu/edk2-x86_64-code.fd

start-uefishell: 
	qemu-system-x86_64 ${USEUEFI} -m 1G

start-uefilinux:
	qemu-system-x86_64 -smp 4 ${USEUEFI} -m 2G -drive format=raw,file=LinuxDisk

start-direct:
	qemu-system-x86_64 -kernel ./uefi-kernel/bzImage.efi \
		           -initrd ./uefi-kernel/initrd.img

connect:
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm ltk:latest /bin/bash

rebuild:
	# cp /Users/zaccaria/development/github/org-teaching/linux-kernel-playground/scripts/uefi-kernel/bzImage ./uefi-kernel/bzImage.efi
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm ltk:latest /local/create-disk.sh
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm ltk:latest /local/write-fs.sh



