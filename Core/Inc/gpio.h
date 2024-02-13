#ifndef GPIO_H
#define GPIO_H

#include "stm32h743xx.h"

void static inline GPIOB_Init() {
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN; // Enable GPIOB clock

  // LD1 (green): GPIOB pin 0
  // LD2 (blue): GPIOB pin 7
  // LD3 (red): GPIOB pin 14

  // Clear output mode bits
  GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE0_Pos);
  GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE7_Pos);
  GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE14_Pos);

  // set to general purpose output mode
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE0_Pos);
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE7_Pos);
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE14_Pos);

  // set to push-pull output mode
  GPIOB->OTYPER &= GPIO_OTYPER_OT0_Msk;
  GPIOB->OTYPER &= GPIO_OTYPER_OT7_Msk;
  GPIOB->OTYPER &= GPIO_OTYPER_OT14_Msk;
}

void static inline Toggle_Pin(GPIO_TypeDef* LED_Port, int LED_Pin) {
    LED_Port->ODR ^= (1 << LED_Pin);
}


#endif // GPIO_H
