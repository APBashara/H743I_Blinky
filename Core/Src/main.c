#include "stm32h743xx.h"
#include "main.h"
#include "gpio.h"

#define GREEN_LED 0
#define BLUE_LED 7
#define RED_LED 14

int main(void) {
  // Call custom GPIO initialization function
  GPIOB_Init();

  while (1) {
    Toggle_Pin(GPIOB, GREEN_LED);
    Toggle_Pin(GPIOB, BLUE_LED);
    Toggle_Pin(GPIOB, RED_LED);

    // Fake Delay
    for (int i = 0; i < 1000000; i++) {
      __asm__("nop");
    }
  }
}