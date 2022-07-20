

firmware.bin: src/main.c
	~/.platformio/penv/bin/platformio run
	cp .pio/build/genericSTM32F407VGT6/firmware.bin $@

run: firmware.bin
	../xpacks/.bin/qemu-system-gnuarmeclipse --verbose --verbose --board STM32F4-Discovery --mcu STM32F407VG -d unimp,guest_errors --image firmware.bin


## private
compile_commands.json: 
	~/.platformio/penv/bin/platformio run -t compiledb
	cp .pio/build/genericSTM32F407VGT6/compile_commands.json ./compile_commands.json
