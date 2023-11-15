# Mini  demo comparing Rust and C
In this demo, we are going to
- Use a system emulation tool called [renode](https://github.com/renode/renode/tree/master), Antmicro's open source simulation and virtual development framework for complex embedded systems
- Install a C compilation environment for the stm32f407 which will allow us to run a C linking led application (we'll log a led blinking on the emulated board). This is based on [Platformio](https://platformio.org/).
- Install the Rust environment to compile for embedded targets and create the same linking led application with Rust (`rled`) using low level API and  using the Rust embedded HAL (`rled-hal`). 
 
# Install the emulator
Install [renode](https://github.com/renode/renode/tree/master) for your platform. On success, you should have a `renode` cli command available. If you have MAC os use the `aliases.sh` script to set it up.

