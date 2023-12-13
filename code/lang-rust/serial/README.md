# STM32F4 introductory demo

This demo instantiates a transmit channel with the external world through the USART2 peripheral. In renode, it is possible setup and to interact with that peripheral as it was a special file on the host (`/tmp/uart`). 

# How to
## Install Rust and renode

```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo install cargo-binutils
# rustup component add llvm-tools-preview (if needed)

make prepare
make firmware.elf

renode renode-script.sh

# In another shell

cat /tmp/uart
# 'Hello'
# 
# 'Hello'
# ...
```
