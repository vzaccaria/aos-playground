build:
	docker build .  -t brk

connect:
	docker run -ti -v `pwd`:/local  --rm brk:latest /bin/bash 



tmp-device-list: 
	~/.platformio/penv/bin/platformio device list

# To allow copy and paste, increase character tx delay to 1ms 
# Meta-Z select 't' select 'f' type '1' to increase character (and also line feed)

tmp-connect: 
	minicom -b 115200 -D /dev/cu.usbmodem2103


# Notes on test mounting image on loopback
# Created image stm32mp1.img from MMC with dd locally
# 
# (local) diskutil list     
# (local) sudo dd if=/dev/disk5 of=stm32mp1.img
# (on connect-dev) losetup -P -r -f stm32mp1.img -> then exit
# (on connect-dev) mount -o ro,noload /dev/loop0p4 /stm32
