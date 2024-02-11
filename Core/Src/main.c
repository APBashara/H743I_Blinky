#include "stm32h743xx.h"
#include "system_stm32h7xx.h"
#include "main.h"

/**
 * @brief Initialize the GPIO pins
 * @note Need PB0 and PE1 for LEDs
 */
void GPIO_Init() {
  // Enable GPIO port B clock
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN | RCC_AHB4ENR_GPIOEEN;

  GPIOB->MODER |= GPIO_MODER_MODE0_0; // Set GPIO mode Output
}

/**
 * @brief Blink the LED
 * @note Uses bitwise XOR to toggle the LED
 */
void LED_Blink() {
  // Toggle PB0
  GPIOB->ODR ^= GPIO_ODR_OD5;
  GPIOE->ODR ^= GPIO_ODR_OD1;
}

/**
 * @brief main function to initialize the system
 * @note Contains the infinite loop
 * @return int 
 */
int main(void) {
  GPIO_Init();

  while (1) {
    LED_Blink();
    for (int i = 0; i < 1000000; i++) {
      __NOP();
    }
  }
}