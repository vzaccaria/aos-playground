#!/bin/sh
qemu-system-aarch64 \
        -M virt \
        -cpu cortex-a57 \
        -smp 1 \
        -nographic \
        -bios u-boot.bin \
        -drive if=none,file=rootfs.ext4.img,format=raw,id=hd \
        -device virtio-blk-device,drive=hd
