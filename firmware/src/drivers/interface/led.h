#ifndef __LED_H__
#define __LED_H__

#include <stdbool.h>

// Led polarity configuration constant
#define LED_POL_POS 0
#define LED_POL_NEG 1

#ifdef PLATFORM_F1
typedef enum {
    LED_GREEN

} led_t;
#elif defined(PLATFORM_F4)
#include "stm32f429i_discovery.h"
// Hardware configuration
#define LED_GPIO_RED       LED3_PIN
#define LED_GPIO_GREEN     LED4_PIN
#define LED_POL_GREEN      LED_POL_NEG
#define LED_POL_RED        LED_POL_NEG
#define LED_GPIO_PORT      GPIOG

#define LED_NUM 2

typedef enum {
    LED_RED = 0,
    LED_GREEN
} led_t;

#endif

void ledInit();
void ledSet(int led, bool value);
void blinkLEDs(void);

#endif
