# Workqueues

This example of the serial driver introduces a workqueue (one of the possible
bottom handlers) which processes received characters. At the moment, the work
queue just prints the characters received and sends them to reading processes.

```
               RBR on
┌────────┐   interrupt
│  UART  │─ ─ ─ ─ ─ ─ ─
└────────┘             ▼                      rxq
              ┌─────────────────┐        ────────┬┬┬┬┬┐
              │   serial_irq    │──────────────▶ ││││││───────┐
              └─────────────────┘        ────────┴┴┴┴┴┘       │
                                                              ▼
                                                     ┌─────────────────┐
┌────────┐                                           │    serial_wq    │
│VFS/read│─ ─ ─ ─ ─ ─ ─                              └─────────────────┘
└────────┘             ▼                      urxq            │
     ▲        ┌─────────────────┐         ┌┬┬┬┬┬────────      │
     └ ─ ─ ─ ─│ serialaos_read  │◀────────││││││◀─────────────┘
              └─────────────────┘         └┴┴┴┴┴────────
```

## Challenge 1: Checking out a workqueue

```
make rebuild
/scripts/start-qemu-serial.sh
(on another shell outside container) nc localhost 6000
insmod modules/serial_aos.ko && mknod -m 600 /dev/serialaos c 248 1

# input characters from the stream should be printed by the async workqueue
# as kernel messages such as
#
# [ 21.211221 ] Received 'a'
# [ 21.213212 ] Received 'b'
#
# even if there isn't a consumer in the user space
...

```

## Challenge 2: Complex processing of serial data through deferred work

This is an open challenge; assume that the data arrives in some format (e.g.,
compressed with some known loss-less algorithm or some numerical time-series)
and implement some sort of processing (e.g, decompression, filtering of data) as
deferred work.

### Resources:

- https://linux-kernel-labs.github.io/refs/heads/master/labs/interrupts.html
- https://www.kernel.org/doc/html/v4.16/driver-api/basics.html
- https://www.kernel.org/doc/html/latest/locking/locktypes.html
- https://alexrhodes.io/blog/post/22/ (good tutorial, but code has some bugs)
- https://en.wikipedia.org/wiki/16550_UART
- https://pccomponents.com/datasheets/NATI-PC16550.pdf
- https://github.com/fedetft/miosix-kernel/blob/master/miosix/arch/common/drivers/serial_lpc2000.cpp
