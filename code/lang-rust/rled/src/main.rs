//! Blinks an LED
//!
//! This assumes that a LED is connected to pc13 as is the case on the blue pill board.
//!
//! Note: Without additional hardware, PC13 should not be used to drive an LED, see page 5.1.2 of
//! the reference manual for an explanation. This is not an issue on the blue pill.

#![no_std]
#![no_main]

use cortex_m::peripheral::{syst::SystClkSource, SYST};
use cortex_m_rt::entry;
use cortex_m_semihosting::hprintln;
use panic_halt as _;
use stm32f4::stm32f407 as pac;

fn init_rcc_and_peripherals() -> (pac::GPIOD, SYST) {
    // Get access to the core peripherals from the cortex-m crate
    let cp = cortex_m::Peripherals::take().unwrap();

    // Get access to the device specific peripherals from the peripheral access crate
    let dp = pac::Peripherals::take().unwrap();
    let rcc = dp.RCC;
    let gpiod = dp.GPIOD;

    // Set up the SysTick peripheral.
    let mut syst = cp.SYST;
    syst.set_clock_source(SystClkSource::Core);
    syst.set_reload(2_100_000); //internal in clock ticks
    syst.enable_counter();

    // Enable clock on port D
    rcc.ahb1enr.write(|w| w.gpioden().set_bit()); //stm32f4
    return (gpiod, syst);
}

// To understand the following, you might want to take a look at
// https://controllerstech.com/stm32-gpio-output-config-using-registers/

#[entry]
fn main() -> ! {
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
}
