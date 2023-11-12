all: firmware.elf

prepare:
	rustup target add thumbv7m-none-eabi

firmware.elf: src/main.rs
	cargo build 
	cargo size --bin rled  -- -A
	cp target/thumbv7m-none-eabi/debug/rled $@

run: firmware.bin
	../xpacks/.bin/qemu-system-gnuarmeclipse --verbose --verbose --board STM32F4-Discovery --mcu STM32F407VG -d unimp,guest_errors --image firmware.bin


