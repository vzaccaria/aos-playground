
# Content
This folder contains various resources for learning about rust embedded development. To run the programs on an emulated board (`stm32f407`), we'll use a system emulation tool called [renode](https://github.com/renode/renode/tree/master), Antmicro's open source simulation and virtual development framework for complex embedded systems.

1. Introduction to rust (`intro` subfolder)
2. Embedded programming with rust
	1. A C led blinking application (`cled` subfolder, uses [Platformio](https://platformio.org/))
	2. A Rust led blinking application 
		- using low level PAC (`rled`) API and 
		- using the embedded HAL API (`rled-hal`). 

# Install the emulator
Install [renode](https://github.com/renode/renode/tree/master) for your platform. On success, you should have a `renode` cli command available. If you have MAC os use the `aliases.sh` script to set it up.

