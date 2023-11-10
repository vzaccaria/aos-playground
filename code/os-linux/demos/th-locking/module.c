#include "linux/time.h"
#include "linux/types.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/nsproxy.h>
#include <linux/pid.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

volatile int64_t shared_variable = 0;
volatile int vl = 10;
volatile uint64_t iter = (1 << 20);

// Broken

// Kthread 1: Adds 1 to the shared variable

static int add_thread_fn(void *data) {
  uint64_t i;
  for (i = 0; i < iter; i++) {
    shared_variable += vl;
  }
  printk("[ADD] finished: %lld\n", shared_variable);
  return 0;
}

// Kthread 2: Decreases 1 from the shared variable
static int subtract_thread_fn(void *data) {
  uint64_t i;
  for (i = 0; i < iter; i++) {
    shared_variable -= vl;
  }
  printk("[SUB] finished: %lld\n", shared_variable);
  return 0;
}

void broken_share(void) {
  kthread_run(add_thread_fn, NULL, "add_thread");
  kthread_run(subtract_thread_fn, NULL, "subtract_thread");
}

// Fixed

atomic64_t a_shared_variable = ATOMIC_INIT(0);
static int a_add_thread_fn(void *data) {
  uint64_t i;
  for (i = 0; i < iter; i++) {
    atomic64_add(vl, &a_shared_variable);
  }
  printk("[ADD] finished: %lld\n", a_shared_variable.counter);
  return 0;
}

static int a_subtract_thread_fn(void *data) {
  uint64_t i;
  for (i = 0; i < iter; i++) {
    atomic64_sub(vl, &a_shared_variable);
  }
  printk("[SUB] finished: %lld\n", a_shared_variable.counter);
  return 0;
}

void fixed_share(void) {
  kthread_run(a_add_thread_fn, NULL, "atomic_add_thread");
  kthread_run(a_subtract_thread_fn, NULL, "atomic_subtract_thread");
}

static int __init my_module_init(void) {
  broken_share();
  /*fixed_share();*/
  pr_info("Module loaded\n");
  return 0;
}

static void __exit my_module_exit(void) { pr_info("Module unloaded\n"); }

module_init(my_module_init);
module_exit(my_module_exit);
