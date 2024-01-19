#include "stm32h743xx.h"
#include "system_stm32h7xx.h"

#define HSE_VALUE 8000000UL // Value of the external oscillator in Hz
#define LSE_VALUE 32768UL // Value of the external oscillator in Hz

/**
 * @brief Configure the basic system clocks
 * @note This function should be called before any other initialization
 * @retval None
 */
void system_config() {
  // https://medium.com/@csrohit/clock-configuration-in-stm32-6a058da220e0
  // PLL Configuration


  // High Speed Clock Configuration
  RCC->CFGR = (3UL << RCC_CFGR_SW_Pos); // Set system clock to HSE
  RCC->CR |= RCC_CR_HSEON; // Enable HSE
  while (!(RCC->CR & RCC_CR_HSERDY)); // Wait until HSE is ready
  
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOEEN; // Enable GPIOE clock

  // Low Speed Clock Configuration
  RCC->BDCR |= RCC_BDCR_LSEON; // Enable LSE
  while(!(RCC->BDCR & RCC_BDCR_LSERDY)); // Wait until LSE is ready
}


/**
 * @brief main function to initialize the system
 * @note Contains the infinite loop
 * @return int 
 */
int main(void) {

  SystemCoreClockUpdate(); // Update the system clock to defined HSE and LSE values

  while (1) {

  }
  return 0;
}