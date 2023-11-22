//! Demo of rust language directly on the STM32F4

#![no_std]
#![no_main]

use core::fmt::Write;
use cortex_m_rt::entry;

mod aux;

use panic_halt as _;

// This sets up interrupt vectors to make the firmware work as expected

// In renode, we are going to use the available UART
// https://github.com/renode/renode/blob/master/platforms/boards/stm32f4_discovery.repl

#[entry]
fn main() -> ! {
    let mut stdout = aux::gen_stdout();
    loop {
        writeln!(stdout, "Hello!\n").unwrap();
    }
}
