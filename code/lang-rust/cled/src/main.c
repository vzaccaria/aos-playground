#include "stm32f4xx_hal.h"

#define LED_PIN GPIO_PIN_12
#define LED_GPIOD_PORT GPIOD
#define LED_GPIOD_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

void init_rcc_and_peripherals() {
  HAL_Init();
  LED_GPIOD_CLK_ENABLE();
}

int main(void) {
  init_rcc_and_peripherals();

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  // This enables the output mode of the pin LED_PIN on PortD->MODER: 0x40020C00
  HAL_GPIO_Init(LED_GPIOD_PORT, &GPIO_InitStruct);

  while (1) {
  // This toggles the pin LED_PIN by 
  // Reading from PortD->ODR: 0x40020C14
  // Writing to PortD->BSRR: 0x40020C18
  //
  // Note that BSRR is comprised of two subregisters of 16 bits each, one for 
  // setting pins high, the other for setting pins low.
    HAL_GPIO_TogglePin(LED_GPIOD_PORT, LED_PIN);
    HAL_Delay(1000);
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }

void NMI_Handler(void) {}

void HardFault_Handler(void) {
  while (1) {
  }
}

void MemManage_Handler(void) {
  while (1) {
  }
}

void BusFault_Handler(void) {
  while (1) {
  }
}

void UsageFault_Handler(void) {
  while (1) {
  }
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}
