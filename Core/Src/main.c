#include "stm32h743xx.h"
#include "main.h"

int main(void) {
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN; // Enable GPIOB clock

  // LD1 (green): GPIOB pin 0
  // LD2 (blue): GPIOB pin 7
  // LD3 (red): GPIOB pin 14

  // Set to output mode
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE0_Pos);
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE7_Pos);
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE14_Pos);
  // set to push-pull output mode
  GPIOB->OTYPER &= GPIO_OTYPER_OT0_Msk;
  GPIOB->OTYPER &= GPIO_OTYPER_OT7_Msk;
  GPIOB->OTYPER &= GPIO_OTYPER_OT14_Msk;


  while (1) {
    GPIOB->ODR ^= GPIO_ODR_OD0;
    GPIOB->ODR ^= GPIO_ODR_OD7;
    GPIOB->ODR ^= GPIO_ODR_OD14;
    HAL_Delay(100);
  }
}