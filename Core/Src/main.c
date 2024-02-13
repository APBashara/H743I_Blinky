#include "stm32h743xx.h"
#include "main.h"

int main(void) {
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN; // Enable GPIOB clock

  // LD1 (green): GPIOB pin 0
  // LD2 (blue): GPIOB pin 7
  // LD3 (red): GPIOB pin 14

  // Set to output mode
  GPIOB->MODER |= GPIO_MODER_MODE0_0;
  GPIOB->MODER &= ~GPIO_MODER_MODE0_1;
  GPIOB->MODER |= GPIO_MODER_MODE7_0;
  GPIOB->MODER &= ~GPIO_MODER_MODE7_1;
  GPIOB->MODER |= GPIO_MODER_MODE14_0;
  GPIOB->MODER &= ~GPIO_MODER_MODE14_1;

  // set to push-pull output mode
  GPIOB->OTYPER &= GPIO_OTYPER_OT0_Msk;
  GPIOB->OTYPER &= GPIO_OTYPER_OT7_Msk;
  GPIOB->OTYPER &= GPIO_OTYPER_OT14_Msk;


  while (1) {
    GPIOB->ODR ^= GPIO_ODR_OD0;
    GPIOB->ODR ^= GPIO_ODR_OD7;
    GPIOB->ODR ^= GPIO_ODR_OD14;
    for (int i = 0; i < 1000000; i++) {
      __asm__("nop");
    }
  }
}