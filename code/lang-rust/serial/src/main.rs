//! Demo of rust language directly on the STM32F4

#![no_std]
#![no_main]

use core::fmt::Write;
use cortex_m_rt::entry;

mod aux;
use panic_halt as _;

fn sum(v: &[i32]) -> i32 {
    let mut m = 0;
    for t in v {
        m += t;
    }
    return m;
}

fn test() -> i32 {
    let x: [i32; 3] = [1, 2, 3];
    return sum(&x[1..2]); // Will cause run-time error
}

#[entry]
fn main() -> ! {
    let mut stdout = aux::gen_stdout();
    loop {
        test();
        writeln!(stdout, "Hello!\n").unwrap();
    }
}
