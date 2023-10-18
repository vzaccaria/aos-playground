Prerequisites
=============
To compile these examples you need to install the appropriate compiler for the Miosix kernel, follow the instructions here
https://miosix.org/wiki/index.php?title=Quick_start

Instructions
============
In a directory different from the one of this repo, open a shell and type:

git clone https://miosix.org/git-public/miosix-kernel.git
cd miosix-kernel
cp <path to aos-playground repo>/code/os-miosix/demos/serialaos/serialaos.patch .
patch -p1 < serialaos.patch
make -j4

You should now have a built firmware (main.bin) to flash on an stm32f4discovery to test.
