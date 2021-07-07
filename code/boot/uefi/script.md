## Kernel compile

- Remember to:
  - turn on `CONFIG_EFI_STUB` and
  - Simple framebuffer support; See here:
    https://wiki.archlinux.org/title/Unified_Extensible_Firmware_Interface but

## How to run

- Enter UEFI shell

  - `ls FS0` should show the efi kernel
  - `FS0:\bootx64.efi initrd=\initrd.img console=ttyS0 init=/init`

## Links used:

- https://wiki.archlinux.org/title/EFISTUB
- https://www.kernel.org/doc/html/v5.4/admin-guide/efi-stub.html
