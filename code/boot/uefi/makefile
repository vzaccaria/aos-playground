
start-uefishell: 
	qemu-system-x86_64 -bios ovmf.fd -m 1G

start-uefilinux:
	qemu-system-x86_64 -bios ovmf.fd -m 1G -drive format=raw,file=LinuxDisk

connect:
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm ltk:latest /bin/bash




