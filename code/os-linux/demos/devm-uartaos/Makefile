obj-m += serial_aos.o

all:
	make -C /sources/linux M=$(PWD) modules

clean:
	make -C /sources/linux M=$(PWD) clean

compile_commands.json: 
	make -C /sources/linux M=$(PWD) $@

prepare: 
	cp /sources/linux/scripts/module.lds.S /sources/linux/scripts/module.lds
	sed -i '$ d' /sources/linux/scripts/module.lds

copy-to-fs: all
	mkdir -p /staging/initramfs/fs/modules
	cp serial_aos.ko /staging/initramfs/fs/modules

rebuild:
	cp .config /sources/linux 
	cd /sources/linux && make -j4 bzImage 
	make prepare 
	make all 
	make copy-to-fs
	/scripts/build-fs.sh 

start:
	/scripts/start-qemu-serial.sh 
		

