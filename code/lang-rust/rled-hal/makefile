all: firmware.elf

prepare:
	rustup target add thumbv7m-none-eabi

firmware.elf: src/main.rs
	cargo build 
	cargo size --bin rled  -- -A
	cp target/thumbv7m-none-eabi/debug/rled $@
