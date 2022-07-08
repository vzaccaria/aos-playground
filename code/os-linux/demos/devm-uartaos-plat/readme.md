# The Linux Device Model framework

This example of the serial driver exploits the
["serial core" linux framework](https://www.kernel.org/doc/Documentation/serial/driver)
and the "platform bus" to implement the same functionality as `devm-uartaos`.

The **platform bus** is a pseudo-bus used to connect devices like integrated
peripherals on many system-on-chip processors which are just present and need to
be statically registered by the kernel when enumerates them and must or should
be integrated into the power management cycle of the system itself (suspend,
resume, shutdown and so on..)

On a SoC platform enumeration happens when the kernel parses _the device tree_
and matches `compatible` fields with data coming from the platform device driver
and invokes the corresponding `probe` method. In turn this invokes the
`platform_device_register` function for the found device, which in turn invokes
the `probe` driver method.

In our case, we want to add `devm-uartaos` to the platform bus. However, we dont
have a device tree so we will invoke `platform_device_register` directly in our
init function.

Nevertheless, the goal of this challenge is to have our device should appear
under `/sys/bus/platform/devices/` as a platform device.

# Challenge n. 1: Checking it out

```
make rebuild
/scripts/start-qemu-serial.sh
insmod modules/serial_aos.ko && mknod -m 600 /dev/serialaos c 248 1
cat /dev/serialaos # type something on the other console
```

(on another shell outside container)

```
nc localhost 6000
(press some characters here.. these should appear over /dev/serialaos
```
