#!/bin/sh
# build linux

mkdir -p /staging/initramfs/fs

cd /staging/initramfs/fs
mkdir -pv bin sbin etc proc sys usr/bin usr/sbin
cp -av /sources/busybox-1.32.1/_install/* .
cp /scripts/init .
find . -print0 | cpio --null -ov --format=newc | gzip -9 > /staging/initramfs-busybox-x86.cpio.gz
cd /staging
