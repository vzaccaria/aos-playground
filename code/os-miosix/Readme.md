> A Miosix kernel playground to guide AOS students towards building the Miosix kernel and learning device driver development in an embedded environment.

## Instructions

- Starting from a bash shell in this directory (on Linux or using the WSL on Windows), try to build the kernel following the challenges and the references.

## Challenges

- Challenge 1: try to install the ARM corss-compiler for Miosix.

  Hint:
  
  ```
  wget https://miosix.org/toolchain/MiosixToolchainInstaller.run
  sh MiosixToolchainInstaller.run
  ```

- Challenge 2: try to download the kernel.
  
  Hint:
  
  ```
  git clone https://github.com/fedetft/miosix-kernel.git
  cd miosix-kernel
  ```

- Challenge 3: try to build the kernel.

  Hint: follow the guide in the references about editing `Makefile.inc` and `miosix_settings.h`

- Challenge 4: try to compile the serialaos example providing a custom serial port driver.

  Hint:
  
  ```
  make clean
  git checkout . # Undo all changes eventually made at challenge 3 to get a clean kernel
  patch -p1 < ../demos/serialaos/serialaos.patch
  make -j`nproc`
  ```

- Challenge 3: if you have a board supported by Miosix, try to run the kernel.
  You should see the boot logs from the serial port.
  Hint (works with most stm32 boards including nucleo and discovery):
  
  ```
  st-flash write main.bin 0x08000000
  screen /dev/ttyUSB0 19200
  ```
  
## References

- https://miosix.org/wiki/index.php?title=Linux_Quick_Start
