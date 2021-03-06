# Mini example comparing Rust and C

In this example, we are going to

- setup a graphical simulation environment for the stm32f407 board. To do this
  we will have to use a special fork of
  [QEMU](https://xpack.github.io/qemu-arm/).

- Install a C compilation environment for the stm32f407 which will allow us to
  run a C linking led application (we'll see a led blinking on the simulated
  board). This is based on [Platformio](https://platformio.org/).

- Install the Rust environment to compile for embedded targets and create the
  same linking led application with Rust.

# Install the graphical simulation environment

You will need nodejs for the following commands to work

```shell
npm install --global xpm@latest

# Install QEMU and ARM toolchain
xpm install
```

which will install qemu in `~/.local/xPacks/\@xpack-dev-tools/qemu-arm/...`

# Install [Platformio](https://platformio.org/) and run the example C blinking app

```
# Install platformio
apt-get install -Y python3-venv
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"

cd cled
make run
```

# Install Rust and run the example Rust blinking app

```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo install cargo-binutils

cd rlet
make prepare
make run

```
