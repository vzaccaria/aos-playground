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
#include <linux/platform_device.h>
#include <linux/serial_core.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vittorio Zaccaria");
MODULE_DESCRIPTION("Simple memory alloc, with kmalloc and vmalloc");

static void *buffer;

#define PN(x) ((void *)((unsigned long long)(x) >> PAGE_SHIFT))

static int print_zones(void) {
  int node_id = numa_node_id();
  struct pglist_data *pgdat = NODE_DATA(node_id);
  int zone_id;

  printk(KERN_INFO "Memory Zones for NUMA Node %d:\n", node_id);

  for (zone_id = 0; zone_id < MAX_NR_ZONES; zone_id++) {
    struct zone *zone = &pgdat->node_zones[zone_id];

    if (zone->present_pages) {
      unsigned long start_pfn = zone->zone_start_pfn;
      unsigned long end_pfn = zone_end_pfn(zone);

      printk(KERN_INFO "Zone %d - Start PPN: 0x%lx, End PPN: 0x%lx\n", zone_id,
             start_pfn, end_pfn);
    }
  }
  return 0;
}

static int alloc_kmalloc(int n) {
  // Calculate the size required for a buffer spanning two pages
  size_t buffer_size = n * PAGE_SIZE;

  // Allocate the buffer with kmalloc
  buffer = kmalloc(buffer_size, GFP_KERNEL);
  if (!buffer) {
    printk(KERN_ERR "Failed to allocate the buffer\n");
    return -ENOMEM;
  }

  // Get the physical addresses of the first and second pages

  printk(KERN_INFO "kmalloc - VPN: %px -> PPN: %px\n\n", PN(buffer),
         PN(virt_to_phys(buffer)));
  kfree(buffer);
  return 0;
}

static int alloc_vmalloc(int n) {
  // Calculate the size required for a buffer spanning two pages
  size_t buffer_size = n * PAGE_SIZE;
  int i;

  // Allocate the buffer with kmalloc
  buffer = vmalloc(buffer_size);
  for (i = 0; i < n; i++) {
    struct page *page = vmalloc_to_page(buffer + i * PAGE_SIZE);
    unsigned long ppn = page_to_pfn(page);
    printk(KERN_INFO "vmalloc - VPN: %px -> PPN: %px\n",
           PN(buffer + i * PAGE_SIZE), (void *)ppn);
  }
  vfree(buffer);
  return 0;
}

static int __init memalloc_init(void) {
  print_zones();
  printk(KERN_INFO "Kernel logical base VPN: %px", PN(PAGE_OFFSET));
  printk(KERN_INFO "Kernel virtual range (VPN - VPN): %px - %px",
         PN(VMALLOC_START), PN(VMALLOC_END));
  alloc_kmalloc(4);
  alloc_vmalloc(4);
  return 0;
}

static void __exit memalloc_cleanup(void) {}

module_init(memalloc_init);
module_exit(memalloc_cleanup);
