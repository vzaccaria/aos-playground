
## Starting the demo

```shell
make rebuild
make start
```

## Description


If you start the module, it will print some information around the current Zones, Address ranges. It will then allocate a few pages with both vmalloc and kmalloc. Note that addresses might change arbitrarily:
```
[   26.081186] memalloc: loading out-of-tree module taints kernel.
[   26.195561] Memory Zones for NUMA Node 0:
[   26.197065] Zone 0 - Start PPN: 0x1, End PPN: 0x1000
[   26.200613] Zone 1 - Start PPN: 0x1000, End PPN: 0x7fe0
[   26.202718] Kernel logical VPN: 000ffff94c080000
[   26.205276] Kernel virtual (VPN - VPN): 000ffffb7c180000 - 000ffffd7c17ffff
[   26.208774] kmalloc - VPN: 000ffff94c081ff8 -> PPN: 0000000000001ff8
[   26.208774] 
[   26.215448] vmalloc - VPN: 000ffffb7c180269 -> PPN: 00000000000020fe
[   26.217798] vmalloc - VPN: 000ffffb7c18026a -> PPN: 0000000000002108
[   26.218680] vmalloc - VPN: 000ffffb7c18026b -> PPN: 00000000000021a1
[   26.219391] vmalloc - VPN: 000ffffb7c18026c -> PPN: 00000000000021a2
```

This shows a few things:
- kmalloc - VPN 000ffff94c081ff8 corresponds to PPN 1ff8, a testimony of the fact that all PPN pages are directly mapped from 000ffff94c080000, i.e. the start of the kernel logical addresses
- vmalloc ppns are not necessarily contiguous

If you print the current **physical** memory mappings you can see that Zones cover only a part of the usable addresses.
```
/ # cat /proc/iomem
00000000-00000fff : Reserved
00001000-0009fbff : System RAM       ↑ 
0009fc00-0009ffff : Reserved         |
000a0000-000bffff : PCI Bus 0000:00  |
000c0000-000c99ff : Video ROM.       |
000ca000-000cadff : Adapter ROM      | Zone 0 (DMA)
000cb000-000cb5ff : Adapter ROM      |
000f0000-000fffff : Reserved         |
  000f0000-000fffff : System ROM     |
00100000-07fdffff : System RAM       ↓ ↑ 
  05400000-0620397f : Kernel code      |
  06400000-0679bfff : Kernel rodata.   | Zone 1 (Normal)
  06800000-06a88d7f : Kernel data.     |
  0707a000-071fffff : Kernel bss.      ↓ 
07fe0000-07ffffff : Reserved
08000000-febfffff : PCI Bus 0000:00
  fd000000-fdffffff : 0000:00:02.0
  feb00000-feb7ffff : 0000:00:03.0
  feb80000-feb9ffff : 0000:00:03.0
    feb80000-feb9ffff : e1000
  febb0000-febb0fff : 0000:00:02.0
fec00000-fec003ff : IOAPIC 0
fed00000-fed003ff : HPET 0
  fed00000-fed003ff : PNP0103:00
fee00000-fee00fff : Local APIC
fffc0000-ffffffff : Reserved
100000000-17fffffff : PCI Bus 0000:00
```

