#include "./serial_aos_circ_buf.h"
#include "linux/jiffies.h"
#include "linux/spinlock_types.h"
#include "linux/workqueue.h"
#include <asm/io.h>
#include <linux/circ_buf.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Federico Terraneo and Vittorio Zaccaria");
MODULE_DESCRIPTION(
    "Serial driver used for teaching ported from Miosix 16550 driver.");

// Stolen from ttyS1, must edit kernel config to free up port from default
// driver
#define PORT_BASE 0x2F8
#define PORT_SIZE 8
#define PORT_IRQ 3

// 16550 registers, see datasheet
#define RBR PORT_BASE + 0
#define THR PORT_BASE + 0
#define IER PORT_BASE + 1
#define IIR PORT_BASE + 2
#define FCR PORT_BASE + 2
#define LSR PORT_BASE + 5

static int major; // Returned when registering our device in /dev
spinlock_t txLock;
static wait_queue_head_t waiting; // For blocking threads waiting on read

/* To the workqueue from interrupt handler */
Q_DECL(rxq, 64, 1);

/* From the workqueue to the process-context reader(s) */
Q_DECL(urxq, 64, 1);

/* This is the deferred work to be done. At the moment it just prints received
 * characters from the interrupt handler and sends them to the waiting
 * process(es)
 */

void serial_wq(struct work_struct *notused) {
  int n = 0;
  int toconsume = 1;
  /* We are in process context now (events/n) so we can use
   * a loop without worrying about the overhead. */
  while (toconsume) {
    char c = 0;
    // note, we are the only consumers so dont need to lock
    // to get it
    Q_GET(rxq, c, toconsume);

    /* Just print out the characters. What about having something more complex
     * done here? i.e., a stream decompressor?
     */
    if (toconsume) {
      printk("Received '%c' \n", (c == '\n') ? '_' : c);
      n++;
    }
    // Send it to user space if any process is waiting
    Q_PUT(urxq, c);
    wake_up(&waiting);
  }
  printk("Received %d chars \n", n);
}

/* Declare our defferred workqueue 'swq' and its handler */
DECLARE_WORK(swq, serial_wq);

static irqreturn_t serial_irq(int irq, void *arg) {
  char c;
  switch (inb(IIR) & 0xf) {
  case 0x6:   // RLS
    inb(LSR); // Read LSR to clear interrupt
    inb(RBR); // Read RBR to discard char that caused error
    return IRQ_HANDLED;
  case 0x4:       // RDA
    c = inb(RBR); // Read LSR to get character and clear interrupt

    // Send it to the work queue
    Q_PUT(rxq, c);
    schedule_work(&swq);

    return IRQ_HANDLED;
  default:
    return IRQ_NONE;
  }
}

static ssize_t serialaos_write(struct file *f, const char __user *buf,
                               size_t size, loff_t *o) {
  int i;
  char c;
  // Simplest possible implementation, poor performance: serial ports are slow
  // compared to the CPU, so using polling to send data one chracter at a time
  // is wasteful. The piece that is missing here is to set up the DMA to send
  // the entire buffer in hardware and give us a DMA end of transfer interrupt
  // when the job is done. We are omitting DMA for simplicity.
  spin_lock(&txLock);
  for (i = 0; i < size; i++) {
    // Copying one byte at a time is slow, can be optimized
    if (copy_from_user(&c, buf + i, 1) != 0) {
      spin_unlock(&txLock);
      return -1;
    }

    while ((inb(LSR) & (1 << 5)) == 0)
      ; // Poll till bit THRE is 1
    outb(c, THR);
  }
  spin_unlock(&txLock);
  return size;
}

static ssize_t serialaos_read(struct file *f, char __user *buf, size_t size,
                              loff_t *o) {
  char c;
  int result;
  int toconsume;
  if (size < 1)
    return 0;

  // Simplest possible implementation, poor performance: this time we DO block
  // waiting for data instead of polling but we return after having read only
  // one character.
  // We should try to fill as many bytes of the buffer as possible, BUT also
  // return prematurely if no more chracter arrive. The piece that is missing
  // here is using the peripheral idle line detection, omitted for simplicity.
  spin_lock_irq(&sl_cons_urxq);
  // Buffer empty? wait for the interrupt
  result =
      wait_event_interruptible_lock_irq(waiting, Q_CNT(urxq) > 0, sl_cons_urxq);
  if (result < 0) {
    // Interrupted by a signal
    spin_unlock_irq(&sl_cons_urxq);
    return result;
  }

  // Get one character from queue
  toconsume = 0;
  Q_GET(urxq, c, toconsume);
  spin_unlock_irq(&sl_cons_urxq);

  if (toconsume == 0 || copy_to_user(buf, &c, 1) != 0)
    return -1;
  return 1; // We read one character
}

static const struct file_operations serialaos_fops = {
    .owner = THIS_MODULE,
    .write = serialaos_write,
    .read = serialaos_read,
};

static int __init hello_init(void) {
  int result;

  init_waitqueue_head(&waiting);
  spin_lock_init(&txLock);

  if (!request_region(PORT_BASE, PORT_SIZE, "serialaos")) {
    // cat /proc/ioports should find who's occupying our port
    pr_info("serialaos: can't access 0x%x\n", PORT_BASE);
    return result;
  }

  result =
      request_irq(PORT_IRQ, serial_irq, IRQF_SHARED, "serialaos", THIS_MODULE);
  if (result < 0) {
    release_region(PORT_BASE, PORT_SIZE);
    pr_info("serialaos: can't claim IRQ %d: %d\n", PORT_IRQ, result);
    return result;
  }

  outb(0x0, FCR); // Disable hardware FIFO
  outb(0x5, IER); // Enable RLS, RDA
  // FIXME: We should set the baud rate but in QEMU it doesn't matter

  major = register_chrdev(0, "serialaos", &serialaos_fops);
  pr_info("serialaos: loaded\n");
  return 0;
}

static void __exit hello_cleanup(void) {
  unregister_chrdev(major, "serialaos");
  release_region(PORT_BASE, PORT_SIZE);
  free_irq(PORT_IRQ, THIS_MODULE);
  pr_info("serialaos: bye\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
