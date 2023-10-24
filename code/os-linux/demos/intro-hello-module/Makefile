obj-m += hello.o

all:
	make -C /sources/linux M=$(PWD) modules

clean:
	make -C /sources/linux M=$(PWD) clean

# Linux stable (5.13) has a problem with the linker script for modules. Here's a quick fix
prepare: 
	cp /sources/linux/scripts/module.lds.S /sources/linux/scripts/module.lds
	sed -i '$ d' /sources/linux/scripts/module.lds

copy-to-fs: all
	mkdir -p /staging/initramfs/fs/modules
	cp hello.ko /staging/initramfs/fs/modules
