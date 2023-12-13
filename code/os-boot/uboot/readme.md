
This folder contains a docker container to create buildroot disk images based on uboot, that are then runnable with qemu. This is inspired by [this post on stack overflow](<Following indications from: https://stackoverflow.com/questions/58028789/how-to-build-and-boot-linux-aarch64-with-u-boot-with-buildroot-on-qemu>).

``` bash
make build
make connect

# in the container
cd /local
make img # create disk image

./start-qemu.sh # mounts image as a file
ext4ls virtio 0 /
```