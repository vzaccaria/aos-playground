# STM32F4 led blink example (low level) 

> This project has been derived from template provided by the [Cortex-M team][team].

## Install Rust and run the example Rust blinking app 

```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo install cargo-binutils
# rustup component add llvm-tools-preview (if needed)

make prepare

make firmware.elf

renode ../renode-script.sh
```

