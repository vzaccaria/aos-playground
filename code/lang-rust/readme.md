# Mini example comparing Rust and C

In this example, we are going to

- setup a graphical simulation environment for the stm32f407 board. To do this
  we will have to use a special fork of
  [QEMU](https://xpack.github.io/qemu-arm/).

- Install a C compilation environment for the stm32f407 which will allow us to
  run a C linking led application (we'll see a led blinking on the simulated
  board).

- Install the Rust environment to compile for embedded targets and create the
  same linking led application with Rust.

# Install the graphical simulation environment

You will need nodejs for the following commands to work

```shell
npm install --global xpm@latest
xpm install --global @xpack-dev-tools/qemu-arm@latest
xpm install --global @xpack-dev-tools/arm-none-eabi-gcc@latest --verbose
```

which will install qemu in `~/.local/xPacks/\@xpack-dev-tools/qemu-arm/...`

# Install platform io and run the example linking app

```
# Install platformio
apt-get install -Y python3-venv
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"

cd cled
make run
```
