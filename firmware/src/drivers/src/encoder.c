#include "encoder.h"
#include "stm32f4xx.h"

static bool isInit;

void encoderInit(void)
{
    if (isInit)  return;

    // Enable bus clock first
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // enable clock of timer2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enable clock of timer3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // enable clock of PORTA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // enable clock of PORTB

    // Config gpio
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // PA15, left encoder phase a
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // PB3, left encoder phase b
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; // PB4, PB5, right encoder phase a b
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Connect the pins to their Alternate Functions
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

    // Set up timer 2 and timer 3 to handle encoder inputs
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,
            TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_SetAutoreload (TIM2, 0xffff);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
            TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_SetAutoreload (TIM3, 0xffff);

    // Reset counter
    TIM_SetCounter(TIM2, 0);
    TIM_SetCounter(TIM3, 0);

    // Enable timers
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

    isInit = true;
}

bool encoderTest(void)
{
    return isInit;
}

int16_t encoderRead(EncoderIdx idx)
{
    int16_t count;

    switch (idx) {
        case LEFT:
            count = (int16_t)TIM2->CNT;
            TIM2->CNT = 0;
            break;
        case RIGHT:
            count = (int16_t)TIM3->CNT;
            TIM3->CNT = 0;
            break;
        default:
            count = 0;
    }

    return count;
}

