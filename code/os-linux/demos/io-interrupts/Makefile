obj-m += module.o

all:
	make -C /sources/linux M=$(PWD) modules

clean:
	make -C /sources/linux M=$(PWD) clean

compile_commands.json:
	make -C /sources/linux M=$(PWD) $@

setup-cc: 
	rm -f compile_commands.json
	make compile_commands.json
	cp compile_commands.json compile_commands.json.in
	cat compile_commands.json.in | sed 's/\-fconserve-stack//g; s/\-fno-stack-clash-protection//g; s/\-mindirect-branch-register//g; s/\-mindirect-branch=thunk-extern//g; s/\-mskip-rax-setup//g; s/\-mpreferred-stack-boundary=3//g; s/\-mno-fp-ret-in-387//g' > compile_commands.json


prepare: 
	cp /sources/linux/scripts/module.lds.S /sources/linux/scripts/module.lds
	sed -i '$ d' /sources/linux/scripts/module.lds

copy-to-fs: all
	mkdir -p /staging/initramfs/fs/modules
	cp module.ko /staging/initramfs/fs/modules

rebuild:
	cp .config /sources/linux 
	cd /sources/linux && make -j4 bzImage 
	make prepare 
	make all 
	make copy-to-fs
	/scripts/build-fs.sh 
	cp /sources/linux/arch/x86_64/boot/bzImage .
	cp /staging/initramfs-busybox-x86.cpio.gz .

start:
	qemu-system-x86_64 -smp 4 -kernel /sources/linux/arch/x86_64/boot/bzImage -initrd /staging/initramfs-busybox-x86.cpio.gz -append "console=ttyS0 init=/init" -vnc :0 -k en-us -nographic
		
start-loc:
	qemu-system-x86_64 -smp 4 -kernel bzImage -initrd initramfs-busybox-x86.cpio.gz -append "init=/init" -k en-us 

