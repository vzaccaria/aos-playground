//! Demo of rust language directly on the STM32F4

#![no_std]
#![no_main]

use core::fmt::Write;
use cortex_m_rt::entry;

use panic_halt as _;

// This sets up interrupt vectors to make the firmware work as expected
use stm32f4xx_hal::{
    pac::{self},
    prelude::*,
    serial::{config::Config, Serial},
};

// In renode, we are going to use the available UART
// https://github.com/renode/renode/blob/master/platforms/boards/stm32f4_discovery.repl

#[entry]
fn main() -> ! {
    let dp = pac::Peripherals::take().unwrap();
    let rcc = dp.RCC.constrain();
    let clocks = rcc.cfgr.use_hse(8_000_000).freeze();
    let gpioa = dp.GPIOA.split();
    let tx_pin = gpioa.pa2.into_alternate();
    let serial_cfg = Config::default()
        .baudrate(115200.bps())
        .wordlength_8()
        .parity_none();

    let usart2 = dp.USART2;
    /* To use a USART, we must what pins it should use because these are configurable */

    let mut stdout = Serial::tx(usart2, tx_pin, serial_cfg, clocks).unwrap();
    loop {
        writeln!(stdout, "Hello!\n").unwrap();
    }
}
