#include "stm32h743xx.h"
#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"

#define GREEN_LED 0
#define BLUE_LED 7
#define RED_LED 14


void BlinkGreenTask(void *argument);
void BlinkRedTask(void *argument);
void SysClockConfig(void);
void Error_Handler(void);

/**
 * @brief Task Thread for blinking the Red LED
 */
osThreadId_t BlinkRedTaskHandle;
const osThreadAttr_t BlinkRedTask_attributes = {
  .name = "BlinkRedTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/**
 * @brief Task Thread for blinking the Green LED
 */
osThreadId_t BlinkGreenTaskHandle;
const osThreadAttr_t BlinkGreenTask_attributes = {
  .name = "BlinkGreenTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/**
 * @brief Configure System Clock for 480MHz clock speed
 * @note Uses PLL1 and PLL2
 * 
 */
void SysClockConfig() {
  // 1. Configure FLASH Latency/Programming Delay
  // 2. Configure PLLs to acheive system clock speed
  // 3. Enable PLLs and wait to be ready
  // 4. Set System Clock Dividers (DIVMx and Prescalars)
  // 5. Select PLL as system clock source

  // Configure FLASH Latency
  // Page 160 RM0433 for Latency values, VOS2 is default
  FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk; // Clear Latency bits
  FLASH->ACR |= FLASH_ACR_LATENCY_4WS; // Set Latency to 4 Wait States

  FLASH->ACR &= ~FLASH_ACR_WRHIGHFREQ_Msk; // Clear Programming Delay bits
  FLASH->ACR |= (2UL << FLASH_ACR_WRHIGHFREQ_Pos); // Set Programming Delay

  // Make sure PLLs are Disabled
  RCC->CR &= ~RCC_CR_PLL1ON; // Disable PLL1
  RCC->CR &= ~RCC_CR_PLL2ON; // Disable PLL2
  RCC->CR &= ~RCC_CR_PLL3ON; // Disable PLL3

  // Configure PLL1
  RCC->PLLCKSELR &= ~RCC_PLLCKSELR_DIVM1_Msk; // Clear DIVM1
  RCC->PLLCKSELR |= (32UL << RCC_PLLCKSELR_DIVM1_Pos); // Set DIVM to /32
  RCC->PLL1DIVR &= ~RCC_PLL1DIVR_R1_Msk; // Clear DIVR
  RCC->PLL1DIVR |= (1UL << RCC_PLL1DIVR_R1_Pos); // Set DIVR to /2
  RCC->PLL1DIVR &= ~RCC_PLL1DIVR_Q1_Msk; // Clear DIVQ
  RCC->PLL1DIVR |= (1UL << RCC_PLL1DIVR_Q1_Pos); // Set DIVQ to /2
  RCC->PLL1DIVR &= ~RCC_PLL1DIVR_P1_Msk; // Clear P1
  RCC->PLL1DIVR |= (1UL << RCC_PLL1DIVR_P1_Pos); // Set P1 to /2
  RCC->PLL1DIVR &= ~RCC_PLL1DIVR_N1_Msk; // Clear DIVN
  RCC->PLL1DIVR |= (480UL << RCC_PLL1DIVR_N1_Pos); // Set DIVN to *480

  // Configure PLL2
  RCC->PLLCKSELR &= ~RCC_PLLCKSELR_DIVM2_Msk; // Clear DIVM2
  RCC->PLLCKSELR |= (32UL << RCC_PLLCKSELR_DIVM2_Pos); // Set DIVM to /32
  RCC->PLL2DIVR &= ~RCC_PLL2DIVR_R2_Msk; // Clear DIVR
  RCC->PLL2DIVR |= (1UL << RCC_PLL2DIVR_R2_Pos); // Set DIVR to /2
  RCC->PLL2DIVR &= ~RCC_PLL2DIVR_Q2_Msk; // Clear DIVQ
  RCC->PLL2DIVR |= (3UL << RCC_PLL2DIVR_Q2_Pos); // Set DIVQ to /4
  RCC->PLL2DIVR &= ~RCC_PLL2DIVR_P2_Msk; // Clear P2
  RCC->PLL2DIVR |= (1UL << RCC_PLL2DIVR_P2_Pos); // Set P2 to /2
  RCC->PLL2DIVR &= ~RCC_PLL2DIVR_N2_Msk; // Clear DIVN
  RCC->PLL2DIVR |= (100UL << RCC_PLL2DIVR_N2_Pos); // Set DIVN to *100

  // Disable PLL3
  // Set DIV bits to 0

  
  // Enable PLLs
  RCC->CR |= RCC_CR_HSION; // Make sure HSI in enabled
  while (!(RCC->CR & RCC_CR_HSIRDY)); // Wait for HSI to be ready
  RCC->CR |= RCC_CR_PLL1ON; // Enable PLL1
  while (!(RCC->CR & RCC_CR_PLL1RDY)); // Wait for PLL1 to be ready
  RCC->CR |= RCC_CR_PLL2ON; // Enable PLL2
  while (!(RCC->CR & RCC_CR_PLL2RDY)); // Wait for PLL2 to be ready
  RCC->CR &= ~RCC_CR_PLL3ON; // Disable PLL3

  // Update System Prescalars
  RCC->D1CFGR |= RCC_D1CFGR_D1PPRE_DIV2; // Set APB3 Prescalar to /2
  RCC->D2CFGR |= RCC_D2CFGR_D2PPRE1_DIV2; // Set APB1 Prescalar to /2
  RCC->D2CFGR |= RCC_D2CFGR_D2PPRE2_DIV2; // Set APB2 Prescalar to /2
  
  // Set System Clock Source to PLL1
  // TODO: Issue is here, system freaks out when trying to switch to PLL
  RCC->CFGR |= (3UL << RCC_CFGR_SW_Pos); // Set System Clock to PLL1
  while (!(RCC->CFGR & RCC_CFGR_SWS_PLL1)); // Wait for PLL1 to be system clock
}

/**
 * @brief Main function to initialize peripherals and start the RTOS
 * @note Do not put anything in the while loop as it should not be reached
 * @return int 
 */
int main(void) {
  SysClockConfig();
  GPIOB_Init();
  osKernelInitialize();

  BlinkRedTaskHandle = osThreadNew(BlinkRedTask, NULL, &BlinkRedTask_attributes);
  BlinkGreenTaskHandle = osThreadNew(BlinkGreenTask, NULL, &BlinkGreenTask_attributes);

  osKernelStart();

  while (1) {
    Error_Handler();
  }
}

/**
 * @brief Task for blinking the Red LED
 * 
 * @param argument 
 */
void BlinkRedTask(void *argument) {
  for (;;) {
    Toggle_Pin(GPIOB, RED_LED);
    osDelay(1000);
  }
}

/**
 * @brief Task for blinking the Green LED
 * 
 * @param argument 
 */
void BlinkGreenTask(void *argument) {
  for (;;) {
    Toggle_Pin(GPIOB, GREEN_LED);
    osDelay(500);
  }
}

/**
 * @brief Error Handler
 * @note TODO: Implement a more detailed error handler
 */
void Error_Handler() {  
  while (1) {}
}