//! Blinks an LED
//!
//! This assumes that a LED is connected to pd13 as is the case on the blue pill board.
//!
//! Note: Without additional hardware, Pd13 should not be used to drive an LED, see page 5.1.2 of
//! the reference manual for an explanation. This is not an issue on the blue pill.

#![no_std]
#![no_main]

use cortex_m::peripheral::syst::SystClkSource;
use cortex_m_rt::entry;
// use cortex_m_semihosting::hprintln;
use panic_halt as _;
use stm32f4xx_hal::{
    pac::{self},
    prelude::*,
};

// To understand the following, you might want to take a look at
// https://controllerstech.com/stm32-gpio-output-config-using-registers/

#[entry]
fn main() -> ! {
    let dp = pac::Peripherals::take().unwrap();

    let mut clk = cortex_m::Peripherals::take().unwrap().SYST;
    clk.set_clock_source(SystClkSource::Core);
    clk.set_reload(2_100_000);
    clk.enable_counter();

    let pin = dp.GPIOD.split().pd13;
    let mut led = pin.into_push_pull_output();

    clk.clear_current();

    loop {
        while !clk.has_wrapped() {}
        // led.toggle();
        led.toggle();
    }
}
