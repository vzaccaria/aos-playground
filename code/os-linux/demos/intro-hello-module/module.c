#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL"); // The license  under which the module is distributed.
MODULE_AUTHOR("Girish Joshi"); // The original author of the module (VZ).
MODULE_DESCRIPTION(
    "HelloWorld Linux Kernel Module."); // The Description of the module.

// This function defines what happens when this module is inserted into the
// kernel. ie. when you run insmod command.
static int __init hello_init(void) {
  printk(KERN_INFO "Hello world!\n");
  return 0; // Non-zero return means that the module couldn't be loaded.
}

// This function defines what happens when this module is removed from the
// kernel. ie.when you run rmmod command.
static void __exit hello_cleanup(void) {
  printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);    // Registers the __init function for the module.
module_exit(hello_cleanup); // Registers the __exit function for the module.
