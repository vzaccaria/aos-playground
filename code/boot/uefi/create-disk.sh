#!/bin/sh
# Check here https://sick.codes/how-to-mount-devices-inside-docker-containers-losetup-loopback-iso-files/
rm -f LinuxDisk
qemu-img create -f raw LinuxDisk 4G
parted LinuxDisk mklabel gpt
parted LinuxDisk mkpart primary 1 2         # 1Mb buffer at begining
parted LinuxDisk mkpart primary 3 102
parted LinuxDisk mkpart primary 103 3220
parted LinuxDisk set 2 boot on
parted LinuxDisk print
losetup -P -f LinuxDisk  # don't know, must exit and reenter


