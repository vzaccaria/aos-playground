
// This sets up interrupt vectors to make the firmware work as expected
use stm32f4xx_hal::{
    pac::{self},
    prelude::*,
    serial::{config::Config, Serial}, 
};

use stm32f4xx_hal::pac::USART2;
use stm32f4xx_hal::serial::Tx;

pub fn gen_stdout() -> Tx<USART2> {
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

    return Serial::tx(usart2, tx_pin, serial_cfg, clocks).unwrap();
}
