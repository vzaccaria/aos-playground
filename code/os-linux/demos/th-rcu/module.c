#include "linux/kern_levels.h"
#include "linux/rcupdate.h"
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vittorio Zaccaria");

// Define a structure for the list elements
struct list_element {
  int data;
  struct list_head list;
};

// Define the list and the lock for synchronization
static LIST_HEAD(my_list);
static DEFINE_SPINLOCK(list_lock);

// In read_list_thread() kthread reads and prints the shared list.
// Notably, spinlock operations that would normally protect against concurrent
// modification are commented out.
static int read_list_thread_norcu(void *data) {
  while (!kthread_should_stop()) {
    struct list_element *entry;
    /* spin_lock(&list_lock); */
    printk(KERN_INFO "[ ");
    list_for_each_entry(entry, &my_list, list) {
      printk(KERN_INFO "%d ", entry->data);
    }
    printk(KERN_INFO "] \n");
    /* spin_unlock(&list_lock); */
    msleep(100); // Sleep for 1 second
  }
  return 0;
}

// In manipulate_list_thread() kthread, it manipulates the shared list by
// removing the first element if any, incrementing its value and adding it back.
// Again, spinlock operations that would prevent corruption due to concurrent
// modifications are commented out.
static int manipulate_list_thread_norcu(void *data) {
  while (!kthread_should_stop()) {
    struct list_element *entry, *temp;
    entry = kmalloc(sizeof(struct list_element), GFP_KERNEL);
    /* spin_lock(&list_lock); */
    if (!list_empty(&my_list)) {
      temp = list_first_entry(&my_list, struct list_element, list);
      list_del(&temp->list);
      entry->data = temp->data + 1;
      kfree(temp);
    }
    list_add(&entry->list, &my_list);
    /* spin_unlock(&list_lock); */
    msleep(200); // Sleep for 2 seconds
  }
  return 0;
}

// read_list_thread_rcu() works similarly to read_list_thread(), but uses RCU
// read-side critical section (via rcu_read_lock() and rcu_read_unlock()) for
// safe iteration through the shared list.
static int read_list_thread_rcu(void *data) {
  while (!kthread_should_stop()) {
    struct list_element *entry;
    rcu_read_lock();
    printk(KERN_INFO "[ ");
    list_for_each_entry(entry, &my_list, list) {
      printk(KERN_INFO "%d ", entry->data);
    }
    printk(KERN_INFO "] \n");
    rcu_read_unlock();
    msleep(100); // Sleep for 1 second
  }
  return 0;
}

struct rcu_head pending_deletes;

// manipulate_list_thread_rcu() works similarly to manipulate_list_thread(), but
// it uses RCU update-side primitives for safe manipulation of the shared list.
// This includes deleting an element with list_del_rcu(), waiting until all
// readers have finished with synchronize_rcu() before kfreeing it, and finally
// adding an element back with list_add_rcu().
//
static int manipulate_list_thread_rcu(void *data) {
  while (!kthread_should_stop()) {
    struct list_element *entry, *temp;
    entry = kmalloc(sizeof(struct list_element), GFP_KERNEL);
    if (!list_empty(&my_list)) {
      temp = list_first_entry(&my_list, struct list_element, list);
      list_del_rcu(&temp->list);
      entry->data = temp->data + 1;
      synchronize_rcu();
      kfree(temp);
    }
    list_add_rcu(&entry->list, &my_list);
    msleep(200); // Sleep for 2 seconds
  }
  return 0;
}

static struct task_struct *read_thread;
static struct task_struct *manipulate_thread;

static int __init my_module_init(void) {
  int i;

  // Initialize the list with 10 elements
  for (i = 0; i < 10; i++) {
    struct list_element *entry =
        kmalloc(sizeof(struct list_element), GFP_KERNEL);
    entry->data = i;
    INIT_LIST_HEAD(&entry->list);
    spin_lock(&list_lock);
    list_add_tail(&entry->list, &my_list);
    spin_unlock(&list_lock);
  }

  // Create and start the kthreads. It uses _rcu (Variant 2) which works.
  // Change from _rcu to _norcu to use Variant 1 (which should break).

  read_thread = kthread_run(read_list_thread_rcu, NULL, "read_list_thread");
  manipulate_thread =
      kthread_run(manipulate_list_thread_rcu, NULL, "manipulate_list_thread");

  /* read_thread = kthread_run(read_list_thread_norcu, NULL,
   * "read_list_thread"); */
  /* manipulate_thread = */
  /*     kthread_run(manipulate_list_thread_norcu, NULL,
   * "manipulate_list_thread"); */

  if (read_thread && manipulate_thread) {
    printk(KERN_INFO "Kernel threads created and started\n");
  } else {
    printk(KERN_ERR "Failed to create kernel threads\n");
    return -ENOMEM;
  }

  return 0;
}

static void __exit my_module_exit(void) {
  struct list_element *entry, *temp;
  if (read_thread && manipulate_thread) {
    kthread_stop(read_thread);
    kthread_stop(manipulate_thread);
  }

  // Clean up the list
  spin_lock(&list_lock);
  list_for_each_entry_safe(entry, temp, &my_list, list) {
    list_del(&entry->list);
    kfree(entry);
  }
  spin_unlock(&list_lock);

  printk(KERN_INFO "Kernel threads stopped and module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
