
# Introduction

This demo shows how to create an interrupt handler. The handler is attached to a keyboard device interrupt (n. 1) in QEMU. Note that QEMU must be start in graphical mode and the kernel options do not contain an external console:

```sh
qemu-system-x86_64 -smp 4 -kernel bzImage -initrd initramfs-busybox-x86.cpio.gz -append "init=/init" -k en-us
``` 