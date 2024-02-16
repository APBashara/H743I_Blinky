#include "stm32h743xx.h"
#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"

#define GREEN_LED 0
#define BLUE_LED 7
#define RED_LED 14


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
 * @brief Main function to initialize peripherals and start the RTOS
 * @note Do not put anything in the while loop as it should not be reached
 * @return int 
 */
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
void Error_Handler(void) {
  while (1) {}
}