
all: firmware.elf
	

firmware.elf: src/main.c
	~/.platformio/penv/bin/platformio run
	cp .pio/build/genericSTM32F407VGT6/firmware.elf $@

show-elf: firmware.elf 
	/Users/zaccaria/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-readelf .pio/build/genericSTM32F407VGT6/firmware.elf -a


## private
compile_commands.json: 
	~/.platformio/penv/bin/platformio run -t compiledb
	# cp .pio/build/genericSTM32F407VGT6/compile_commands.json ./compile_commands.json
