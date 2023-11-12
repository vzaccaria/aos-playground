# Mini  demo comparing Rust and C
In this demo, we are going to
- Use a system emulation tool called [renode](https://github.com/renode/renode/tree/master), Antmicro's open source simulation and virtual development framework for complex embedded systems
- Install a C compilation environment for the stm32f407 which will allow us to run a C linking led application (we'll log a led blinking on the emulated board). This is based on [Platformio](https://platformio.org/).
- Install the Rust environment to compile for embedded targets and create the same linking led application with Rust.

# Install the emulator
Install [renode](https://github.com/renode/renode/tree/master) for your platform. On success, you should have a `renode` cli command available.

# Install [Platformio](https://platformio.org/) and compile the example C blinking app

```shell
# you might need to install python3.10-venv
wget -O get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py

cd cled
make firmware.elf
```

# Install Rust and run the example Rust blinking app

```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo install cargo-binutils
# rustup component add llvm-tools-preview (if needed)

cd rled
make prepare
make run

```
