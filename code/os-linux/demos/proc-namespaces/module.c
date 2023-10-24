#include "asm/page_types.h"
#include "linux/kern_levels.h"
#include "linux/vm_event_item.h"
#include <asm/io.h>
#include <asm/page.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/module.h>
#include <linux/pid_namespace.h>
#include <linux/platform_device.h>
#include <linux/serial_core.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/nsproxy.h>
#include <linux/pid.h>
#include <linux/sched.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pid_namespace.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static int __init pid_printer_start(void) {
  struct task_struct *task;
  struct pid_namespace *ns;

  printk(KERN_INFO "Pid Printer Module Loaded.\n");

  for_each_process(task) {
    printk(KERN_INFO "Global PID: %d\n", task->pid);

    for (ns = task_active_pid_ns(task); ns; ns = ns->parent)
      printk(KERN_INFO "PID in namespace (level %d): %d\n", ns->level,
             pid_nr_ns(task_pid(task), ns));
  }

  return 0;
}

static void __exit pid_printer_end(void) {
  printk(KERN_INFO "Pid Printer Module Unloaded.\n");
}

module_init(pid_printer_start);
module_exit(pid_printer_end);
