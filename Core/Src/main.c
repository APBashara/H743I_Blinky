#include "stm32h743xx.h"
#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"

#define GREEN_LED 0
#define BLUE_LED 7
#define RED_LED 14


// static volatile uint32_t ms_tick = 0 ; // TODO Systick Counter

/**
 * @brief Stack Overflow Delay function
 * @note TODO
 */
// void SysTick_Init(void)  {
//     SysTick_Config( SystemCoreClock / 1000 ) ;  
// }

// void SysTick_Handler(void) {
//     ms_tick++;
// }

// void delayMs( uint32_t delay) {
//     uint32_t start_tick = ms_tick ;
//     while( (ms_tick - start_tick) < delay );
// }
void BlinkGreenTask(void *argument);
void BlinkRedTask(void *argument);
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
 * @brief Quick and dirty delay function
 * @note TODO: Make a better delay function
 */
void delay(uint32_t delay) {
  for (volatile int i = 0; i < delay; i++) {
    __asm__("nop");
  }
}


int main(void) {
  // Call custom GPIO initialization function
  GPIOB_Init();
  osKernelInitialize();

  BlinkRedTaskHandle = osThreadNew(BlinkRedTask, NULL, &BlinkRedTask_attributes);
  BlinkGreenTaskHandle = osThreadNew(BlinkGreenTask, NULL, &BlinkGreenTask_attributes);

  osKernelStart();

  while (1) {
    Error_Handler();
  }
}

void BlinkRedTask(void *argument) {
  for (;;) {
    Toggle_Pin(GPIOB, RED_LED);
    osDelay(1000);
  }
}

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
void Error_Handler(void) {
  while (1) {}
}