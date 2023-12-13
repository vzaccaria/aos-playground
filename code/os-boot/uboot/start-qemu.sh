#!/bin/sh
qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -smp 1 -bios u-boot.bin -netdev user,id=eth0 -device virtio-net-device,netdev=eth0 -drive file=rootfs.ext4.img,if=none,format=raw,id=hd0 -device virtio-blk-device,drive=hd0 
