#include "stm32h743xx.h"
#include "system_stm32h7xx.h"

#define HSE_VALUE 8000000UL // Value of the external oscillator in Hz
#define LSE_VALUE 32768UL // Value of the external oscillator in Hz

/**
 * @brief Manual clock configuration
 * @note This function should be called before any other initialization
 * @retval None
 */
void System_Config() {
   // Enable HSE
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));

    // Configure PLL
    RCC->PLLCKSELR = RCC_PLLCKSELR_PLLSRC_HSE | // HSE as PLL source
                     (1UL << RCC_PLLCKSELR_DIVM1_Pos); // Divide by 1 TODO

    RCC->PLLCFGR = RCC_PLLCFGR_PLL1VCOSEL | // Select wide VCO range: 192 to 836 MHz
                   (2UL << RCC_PLLCFGR_PLL1RGE_Pos) | // Select input frequency range: 8 to 16 MHz
                   RCC_PLLCFGR_DIVR1EN | // Enable PLL1 divider R
                   RCC_PLLCFGR_DIVN1(120); // Multiply by 120

    RCC->PLL1DIVR = RCC_PLL1DIVR_DIVP1(2); // Divide by 2 for PLLCLK (will be 480MHz)

    // Enable PLL
    RCC->CR |= RCC_CR_PLL1ON;
    while(!(RCC->CR & RCC_CR_PLL1RDY));

    // Select PLL as system clock
    RCC->CFGR |= RCC_CFGR_SW_PLL1;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL1);

    SystemCoreClockUpdate(); // Update the SystemCoreClock variable
}

void GPIO_Config() {
  // Enable GPIOA clock
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;

  // Configure PA5 as output
  GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
  GPIOA->MODER |= GPIO_MODER_MODE5_0;
}

void LED_Blink() {
  // Toggle PA5
  GPIOA->ODR ^= GPIO_ODR_OD5;
}

/**
 * @brief main function to initialize the system
 * @note Contains the infinite loop
 * @return int 
 */
int main(void) {
  
  System_Config();

  while (1) {
    LED_Blink();
    HAL_Delay(1000);
  }
  return 0;
}