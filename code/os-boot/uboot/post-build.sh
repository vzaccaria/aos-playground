#!/usr/bin/env bash
cp /root/buildroot/output/images/Image ${TARGET_DIR}/vmlinux
cp /local/qemu.dtb ${TARGET_DIR}/qemu.dtb
