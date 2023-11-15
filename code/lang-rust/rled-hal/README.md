# STM32F4 led blink example (HAL level) 

> This project has been derived from template provided by the [Cortex-M team][team] and inspired by [this blog post](https://dev.to/apollolabsbin/stm32f4-embedded-rust-at-the-hal-gpio-button-controlled-blinking-20a3)

## Install Rust and run the example Rust blinking app 

```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo install cargo-binutils
# rustup component add llvm-tools-preview (if needed)

cd rled
make prepare
make firmware.elf

renode ../renode-script.sh
```

## Ok, what is the difference with the `rled` demo?

The first evident difference involves the abstractions used in manipulating the LED pin. For instance, in `rled`, you would need to write this contrived code:
```rust
    let (gpiod, mut syst) = init_rcc_and_peripherals();

    gpiod.moder.write(|w| w.moder13().output());
    gpiod.otyper.write(|w| w.ot13().push_pull());
    gpiod.ospeedr.write(|w| w.ospeedr13().low_speed());
    gpiod.pupdr.write(|w| w.pupdr13().floating());

    // Restart the SysTick counter.
    syst.clear_current();
    hprintln!("Hello, world!").unwrap();

    loop {
        // Toggle the LED every SysTick tick.
        // This writes directly to GPIOD->ODR f
        while !syst.has_wrapped() {}
        gpiod.odr.write(|w| w.odr13().set_bit());
        while !syst.has_wrapped() {}
        gpiod.odr.write(|w| w.odr13().clear_bit());
    }

```

while in `rled-hal` you just manipulate the `Pin` type:

```rust
    let pin = dp.GPIOD.split().pd13;   // Pin<Input<Floating>, 'D', 13>
    let mut led = pin.into_push_pull_output(); // Pin<Output<PushPull>, 'D', 13>

    clk.clear_current();

    loop {
        while !clk.has_wrapped() {}
        led.toggle();
    }
```

Second of all, note the type of `led` and `pin`. They are both `Pin`, but they encode the state of the pin in their type; you cannot toggle a pin that is not in the state `Output<_>` and you have it only if you explicitly invoke `into_push_pull_output()` . This kind of pattern (called  ['type state programming'](https://docs.rust-embedded.org/book/static-guarantees/state-machines.html)) forces you to treat peripherals as state machines and forbids meaningless operations on states (e.g., write to an input). 