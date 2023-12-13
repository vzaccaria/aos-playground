echo Executing GDB with .gdbinit to connect to OpenOCD.\n
echo .gdbinit is a hidden file. Press Ctrl-H in the current working directory to see it.\n
# Connect to OpenOCD
target extended-remote localhost:3333

# Reset the target and call its init script
monitor reset halt 

# Enable semihosting
monitor arm semihosting enable

