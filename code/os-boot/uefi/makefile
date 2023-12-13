USEUEFI=-drive if=pflash,format=raw,read-only=on,file=/opt/homebrew/Cellar/qemu/8.1.0/share/qemu/edk2-x86_64-code.fd 

start-uefishell: 
	qemu-system-x86_64 ${USEUEFI} -m 1G

start-uefilinux: LinuxDisk
	qemu-system-x86_64 -smp 4 ${USEUEFI} -m 2G -drive format=raw,file=LinuxDisk

connect:
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm lkp-dev:latest /bin/bash

LinuxDisk: ./create-disk.sh ./write-fs.sh
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm lkp-dev:latest /local/create-disk.sh
	docker run -ti --privileged -v `pwd`:/local  -w /local --rm lkp-dev:latest /local/write-fs.sh



