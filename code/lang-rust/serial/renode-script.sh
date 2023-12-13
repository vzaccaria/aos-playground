# check https://github.com/renode/renode/blob/master/platforms/boards/stm32f4_discovery.repl
mach create
machine LoadPlatformDescription @platforms/boards/stm32f4_discovery-kit.repl
sysbus LoadELF $CWD/firmware.elf
emulation CreateUartPtyTerminal "term" "/tmp/uart"
connector Connect sysbus.uart2 term
start
