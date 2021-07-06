Links used:

- https://wiki.archlinux.org/title/EFISTUB
- https://www.kernel.org/doc/html/v5.4/admin-guide/efi-stub.html

- Enter UEFI shell

  - `ls FS0` should show the efi kernel
  - `FS0:\bzImage.efi initrd=\initrd.img`
