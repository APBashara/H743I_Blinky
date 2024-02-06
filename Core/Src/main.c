#include "stm32h743xx.h"
#include "main.h"

int main(void) {
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN; // Enable GPIOB clock

  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE8_Pos);
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE9_Pos);
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT8;
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT9;

  while (1) {
    GPIOB->ODR ^= GPIO_ODR_OD8;
    GPIOB->ODR ^= GPIO_ODR_OD9;
    for (int i = 0; i < 1000000; i++) {
      __asm__("nop");
    }
  }
}