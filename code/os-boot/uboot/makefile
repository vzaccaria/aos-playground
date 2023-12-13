build:
	docker build .  -t brk

connect:
	docker run -ti -v `pwd`:/local  --rm brk:latest /bin/bash 


img: 
	cp dot.config /root/buildroot/.config
	make -C /root/buildroot 
	cp /root/buildroot/output/images/rootfs.ext4 .
	cp /root/buildroot/output/images/u-boot.bin .
	./sfdisk-fs-to-img.sh rootfs.ext4

clean: 
	rm -f rootfs.* u-boot.bin


