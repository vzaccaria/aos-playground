
This demo is composed of two parts.

# Part 1 - hop into the uefi shell and play around

``` bash
(host)> make start-uefishell 
# press immediately "F2" when Tiano white box appears, then release 
# Go to boot manager > EFI Internal Shell

(efi)> 
(efi)> help -b # (all commands in the shell)
(efi)> dmem    # shows also the address of the ACPI table 
# shows address of SMBIOS, an alternative and simpler interface for getting 
# computer configuration
(efi)> pci     # shows the pci devices attached (note bus/dev/func geographical addressing)  
(efi)> pci 00 02 00 -i # shows command and status fields of the bar 
(efi)> devices, devtree, drivers # UEFI drivers
```


# Part 2 - (Optional) boot a Linux EFI image with its own Initrd

This somewhat boots Linux. It is however broken because it stops in the middle of booting :-)

![](Images/readme%202023-12-14%2015.39.37.excalidraw.png)
%%[🖋 Edit in Excalidraw](Images/readme%202023-12-14%2015.39.37.excalidraw.md)%%

``` bash
(host)> make start-uefilinux
# press immediately "F2" when Tiano white box appears, then release 
# Go to boot manager > EFI Internal Shell

(efi)> 
(efi)> ls FS0 # should show the efi kernel
(efi)> FS0:\bootx64.efi initrd=\initrd.img init=/init # should boot linux
```

## Disk construction

The LinuxDisk gpt image file must be built with `make LinuxDisk` (check out the makefile)

## Links used:

- https://wiki.archlinux.org/title/EFISTUB
- https://www.kernel.org/doc/html/v5.4/admin-guide/efi-stub.html
