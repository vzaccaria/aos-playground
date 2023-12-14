

This folder contains a docker container to create buildroot disk images based on UBOOT.

![](Images/readme%202023-12-14%2015.40.40.excalidraw.png)
%%[🖋 Edit in Excalidraw](Images/readme%202023-12-14%2015.40.40.excalidraw.md)%%

When you create the container, it will download all the necessary tools to compile a kernel form arm64, so it will take a while.
``` bash
(host)> make build
```

To build the disk image used for the demonstration:

``` bash
(host)> make connect # local directoy mounted as /local

(container)> cd /local 
(container)> make img # create disk image
(container)> exit 

# you should now find rootfs.ext4.img and u-boot.bin
```

To run the simulation and boot into UBOOT command line:
```shell

(host)> ./start-qemu.sh # launch qemu
...

(uboot)> part list virtio 0 
(uboot)> fdt addr 0x0000000046dd7dd0 # bdinfo > fdt_blob
(uboot)> fdt list /cpus/cpu@0
```

To boot the Linux image from the file system with the generated rootfs:

```shell
(uboot)> ext4load virtio 0 ${kernel_addr_r} /vmlinux
(uboot)> setenv -f bootargs "root=/dev/vda1 rootwait rw" 
(uboot)> booti ${kernel_addr_r} - 0x0000000046dd7dd0
```


This demo has been inspired by [this post on stack overflow](<Following indications from: https://stackoverflow.com/questions/58028789/how-to-build-and-boot-linux-aarch64-with-u-boot-with-buildroot-on-qemu>).
