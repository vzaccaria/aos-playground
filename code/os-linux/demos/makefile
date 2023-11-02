
DIRS:=intro-hello-module proc-namespaces th-locking vm-memalloc io-interrupts 

build-all:
	@for dir in $(DIRS); do \
        echo "Building module: $$dir"; \
        cd $$dir && make copy-to-fs && cd -; \
        done

clean-all:
	@for dir in $(DIRS); do \
        echo "Building module: $$dir"; \
        cd $$dir && make clean && cd -; \
        done

build-kernel:
	cp .config /sources/linux
	cd /sources/linux && make -j4 bzImage
	cp /sources/linux/scripts/module.lds.S /sources/linux/scripts/module.lds
	sed -i '$ d' /sources/linux/scripts/module.lds
	/scripts/build-fs.sh
	cp /sources/linux/arch/x86_64/boot/bzImage .
	cp /staging/initramfs-busybox-x86.cpio.gz .


start: 
	qemu-system-x86_64 -smp 4 -kernel /sources/linux/arch/x86_64/boot/bzImage -initrd /staging/initramfs-busybox-x86.cpio.gz -append "console=ttyS0 init=/init" -vnc :0 -k en-us -nographic


start-loc:
	qemu-system-x86_64 -smp 4 -kernel bzImage -initrd initramfs-busybox-x86.cpio.gz -append "init=/init" -k en-us


