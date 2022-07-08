# The Linux Device Model framework

This example of the serial driver exploits the
["serial core" linux framework](https://www.kernel.org/doc/Documentation/serial/driver)
to implement the same functionality as the plain `serialaos` driver but with
much less effort.

In practice, one must provide an handler for the character received interrupt
and a `start_tx` transmission function as part of a `uart_port` definition.
Everything else is set up by the framework (i.e., the circular buffer(s), the
character device and so on..)

To start it up:

# Challenge n. 1: Checking it out

```
make rebuild
/scripts/start-qemu-serial.sh
insmod modules/serial_aos.ko && mknod -m 600 /dev/serialaos c 248 1
echo "testing" > /dev/serialaos # Should see the data coming out from the socket on the other shell
cat /dev/serialaos # Whet you type on the other shell should be printed
```

(on another shell outside container)

```
nc localhost 6000
(press some characters here.. these should appear over /dev/serialaos
```

