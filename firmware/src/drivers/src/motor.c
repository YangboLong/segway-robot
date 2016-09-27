#include "motor.h"
#include "pwm.h"

static TM_PWM_TIM_t TIM4_Data;
static bool isInit;

void motorInit(void)
{
    if(isInit)  return;

    /* Initialize system */
    SystemInit();

    /* Set PWM to 10kHz frequency on timer TIM4. */
    TM_PWM_InitTimer(TIM4, &TIM4_Data, 10000);
    /* Initialize PWM on TIM4, Channel 1 and PinsPack 1 = PB6 */
    TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_1);
    /* Initialize PWM on TIM4, Channel 2 and PinsPack 1 = PB7 */
    TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);

    /* Initial PWM percentage is set to 0 */
    TM_PWM_SetChannelPercent(&TIM4_Data, TM_PWM_Channel_1, 0);
    TM_PWM_SetChannelPercent(&TIM4_Data, TM_PWM_Channel_2, 0);

    isInit = true;
}

void powerDistribution(void)
{
    /* Set channel 1 value, 50% duty cycle */
    TM_PWM_SetChannel(&TIM4_Data, TM_PWM_Channel_1, TIM4_Data.Period / 2);
    /* Set channel 2 value, 33% duty cycle */
    TM_PWM_SetChannel(&TIM4_Data, TM_PWM_Channel_2, TIM4_Data.Period / 4);
}

bool motorTest(void)
{
    return isInit;
}
