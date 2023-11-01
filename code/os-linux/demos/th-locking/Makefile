obj-m += module.o

all:
	make -C /sources/linux M=$(PWD) modules

clean:
	make -C /sources/linux M=$(PWD) clean

compile_commands.json: 
	make -C /sources/linux M=$(PWD) $@

backup-cc:
	cp compile_commands.json compile_commands.json.in

rewrite-cc: compile_commands.json.in 
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

start:
	/scripts/start-qemu.sh 
		

