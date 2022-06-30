#!/bin/sh
# build linux

mkdir -p /staging/initramfs/fs

cd /sources/linux
make x86_64_defconfig
make -j4 bzImage


# build busybox
cd /sources/busybox-1.32.1
make defconfig
LDFLAGS="--static" make -j4 install

