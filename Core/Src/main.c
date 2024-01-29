#include "stm32h743xx.h"
#include "system_stm32h7xx.h"
#include "main.h"

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
                   RCC_PLLCFGR_DIVR1EN; // Enable PLL1 divider R

    RCC->PLL1DIVR = (120UL << RCC_PLL1DIVR_N1_Pos) | // Multiply by 120
                    (2UL << RCC_PLL1DIVR_P1_Pos); // Divide by 2

    // Enable PLL
    RCC->CR |= RCC_CR_PLL1ON;
    while(!(RCC->CR & RCC_CR_PLL1RDY));

    // Select PLL as system clock
    RCC->CFGR |= RCC_CFGR_SW_PLL1;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL1);

    SystemCoreClockUpdate(); // Update the SystemCoreClock variable
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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