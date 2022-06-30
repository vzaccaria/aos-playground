CC=docker run -v $(PWD):/root/local -w /root/local --rm --entrypoint "/opt/riscv/toolchain/bin/riscv64-unknown-elf-gcc" xv6rv -g
OBJDUMP=docker run -v $(PWD):/root/local -w /root/local --rm --entrypoint "/opt/riscv/toolchain/bin/riscv64-unknown-elf-objdump" xv6rv
NM=docker run -v $(PWD):/root/local -w /root/local --rm --entrypoint "/opt/riscv/toolchain/bin/riscv64-unknown-elf-nm" xv6rv
RUN=docker run -v $(PWD):/root/local -w /root/local --rm --entrypoint '/usr/local/bin/rv-sim' xv6rv
GDB=docker run -ti -v $(PWD):/root/local -w /root/local --rm --entrypoint '/opt/riscv/toolchain/bin/riscv64-unknown-elf-gdb' xv6rv
READELF=docker run -v $(PWD):/root/local -w /root/local --rm --entrypoint "/opt/riscv/toolchain/bin/riscv64-unknown-elf-readelf" xv6rv

