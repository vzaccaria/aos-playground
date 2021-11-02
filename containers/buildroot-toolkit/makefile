build:
	docker build .  -t brk

connect:
	docker run -ti -v `pwd`:/local  --rm brk:latest /bin/bash 


tmp-device-list: 
	~/.platformio/penv/bin/platformio device list

tmp-connect: 
	picocom -b 115200 /dev/cu.usbmodem14432
