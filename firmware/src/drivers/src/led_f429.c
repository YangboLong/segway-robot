#include <stdbool.h>
#include "led.h"

#ifdef PLATFORM_F1
static GPIO_TypeDef *led_port[] = {

};
static unsigned int led_pin[] = {

};
static int led_polarity[] = {

};
#elif defined(PLATFORM_F4)
static GPIO_TypeDef *led_port[] = {
    [LED_GREEN] = LED_GPIO_PORT,
    [LED_RED]   = LED_GPIO_PORT
};
static unsigned int led_pin[] = {
    [LED_GREEN] = LED_GPIO_GREEN,
    [LED_RED]   = LED_GPIO_RED
};
static int led_polarity[] = {
    [LED_GREEN] = LED_POL_GREEN,
    [LED_RED]   = LED_POL_RED
};
#endif

static bool isInit = false;

static void delay(__IO uint32_t nCount)
{
    while(nCount--);
}

void ledInit()
{
    int i;
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    for (i = 0; i < LED_NUM; i++) {
        // Initialize the LED pins as an output
        GPIO_InitStructure.GPIO_Pin   = led_pin[i];
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
        GPIO_Init(led_port[i], &GPIO_InitStructure);

        // Turn off the LEDs
        ledSet(i, 0);
    }

    isInit = true;
}

void ledSet(int led, bool value)
{
    if (led > LED_NUM)  return;

    if (led_polarity[led] == LED_POL_NEG)
        value = !value;

    if(value)
        GPIO_SetBits(led_port[led], led_pin[led]);
    else
        GPIO_ResetBits(led_port[led], led_pin[led]);
}

void blinkLEDs(void)
{
    int i;
    for (i = 0; i < LED_NUM; i++) {
        ledSet(i, 1);
    }
    delay(1000000);

    for (i = 0; i < LED_NUM; i++) {
        ledSet(i, 0);
    }
    delay(1000000);
}
