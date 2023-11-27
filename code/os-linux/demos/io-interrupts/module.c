#include "linux/notifier.h"
#include "linux/time.h"
#include "linux/types.h"
#include <asm/io.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/nsproxy.h>
#include <linux/pid.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

#define KBD_DATA_PORT 0x60 // PS/2 keyboard data port
#define KBD_DATA_SIZE 0x1  //

irqreturn_t keyboard_interrupt(int irq, void *dev_id) {
  unsigned char scancode = inb(KBD_DATA_PORT);

  // Output the scancode to the kernel log
  printk(KERN_INFO "Keyboard scancode: 0x%02x\n", scancode);
  printk(KERN_INFO "Keyboard in\n");

  return IRQ_HANDLED;
}

// Remember to run this in QEMU graphical, without console (see Makefile)
// Otherwise the keyboard used is not the standard one that sends int 1
// interrupts

static int __init keyboard_init(void) {
  int irq = 1; // Keyboard interrupt line

  request_region(KBD_DATA_PORT, KBD_DATA_SIZE, "keyboard_drv");

  // Request the keyboard interrupt. You use IRQF_SHARED when you think that irq
  // could be used potentially by other devices.
  if (request_irq(irq, keyboard_interrupt, IRQF_SHARED, "keyboard_isr",
                  (void *)&irq)) {
    printk(KERN_ERR "Failed to request IRQ %d\n", irq);
    return -1;
  }

  printk(KERN_INFO "Keyboard module loaded\n");

  return 0;
}

static void __exit keyboard_exit(void) {
  int irq = 1; // Keyboard interrupt line

  // Free the keyboard interrupt
  free_irq(irq, (void *)&irq);

  printk(KERN_INFO "Keyboard module unloaded\n");
}

module_init(keyboard_init);
module_exit(keyboard_exit);
