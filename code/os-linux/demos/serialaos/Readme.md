
## Challenge 1: adding a second serial port in QEMU

TL;DR use the `/scripts/start-qemu-serial.sh` script, and run `nc localhost 6000` in a second shell outside the docker container. The second serial port can be accessed from the second shell.

With basic QEMU there's only one serial port

```
qemu-system-x86_64 -smp 4 -kernel /sources/linux/arch/x86_64/boot/bzImage
                          -initrd /staging/initramfs-busybox-x86.cpio.gz
                          -append "console=ttyS0 init=/init" -vnc :0 -k en-us
                          -nographic

/ # dmesg | grep tty
[    0.000000] Command line: console=ttyS0 init=/init
[    0.000000] Kernel command line: console=ttyS0 init=/init
[    0.000000] printk: console [ttyS0] enabled
[    2.114513] 00:05: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 16550A
```

Adding the option to enable the second one (requires to run `nc localhost 6000` on a second shell not inside the docker container to start the VM)

```
qemu-system-x86_64 -smp 4 -kernel /sources/linux/arch/x86_64/boot/bzImage
                          -initrd /staging/initramfs-busybox-x86.cpio.gz
                          -append "console=ttyS0 init=/init" -vnc :0 -k en-us
                          -serial stdio -serial tcp::6000,server

/ # dmesg | grep tty
[    0.000000] Command line: console=ttyS0 init=/init
[    0.000000] Kernel command line: console=ttyS0 init=/init
[    0.000000] printk: console [ttyS0] enabled
[    2.244087] 00:05: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 16550A
[    2.246196] 00:06: ttyS1 at I/O 0x2f8 (irq = 3, base_baud = 115200) is a 16550A
```

Unfortunately by doing so we lose the possibility to stop the VM with CTLR-a x. Instead, we must use CTRL-c.

## Challenge 2: testing the second serial port using the default Linux serial driver

Our minimal Linux environment lacks udev, so we need to create the ddevice file in `/dev` ourselves with mknod.

```
/scripts/build-fs.sh
/scripts/start-qemu-serial.sh
(on another shell outside container) nc localhost 6000
cat /proc/devices # We learn the major number for ttyS is 4
mknod -m 600 /dev/ttyS1 c 4 65
ls -l /dev
stty -F /dev/ttyS1 # Allows to see serial port parameters
echo "testing" > /dev/ttyS1 # Should see the data coming out from the socket on the other shell
cat /dev/ttyS1 # Whet you type on the other shell should be printed
```

## Challenge 3: disabling the default Linux kernel driver for the second serial port and use the seialaos driver

```
cd /sources/linux
make menuconfig
# > Device Drivers > Character devices > Serial drivers >
# Maximum number of 8250/16550 serial ports (1)
# Number of 8250/16550 serial ports to register at runtime (1)
# DON'T USE the build-k.sh script, it overwrites the config as it does make defconfig!
make -j4
make -j4 bzImage
cd /demos/serialaos
make prepare
make all
make copy-to-fs
/scripts/build-fs.sh
/scripts/start-qemu-serial.sh
(on another shell outside container) nc localhost 6000
insmod modules/serial_aos.ko
cat /proc/devices # We learn the major number for serialaos is 248
mknod -m 600 /dev/serialaos c 248 1
ls -l /dev
echo "testing" > /dev/serialaos # Should see the data coming out from the socket on the other shell
cat /dev/serialaos # Whet you type on the other shell should be printed

```

### Resources:
- https://linux-kernel-labs.github.io/refs/heads/master/labs/interrupts.html
- https://www.kernel.org/doc/html/v4.16/driver-api/basics.html
- https://www.kernel.org/doc/html/latest/locking/locktypes.html
- https://alexrhodes.io/blog/post/22/ (good tutorial, but code has some bugs)
- https://en.wikipedia.org/wiki/16550_UART
- https://pccomponents.com/datasheets/NATI-PC16550.pdf
- https://github.com/fedetft/miosix-kernel/blob/master/miosix/arch/common/drivers/serial_lpc2000.cpp
