#include "stm32f10x.h"

#define LED_ON()   GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LED_OFF()  GPIO_SetBits(GPIOA, GPIO_Pin_8)

void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
}

void Initial_LED_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

int main(void)
{

  SystemInit();
	Initial_LED_GPIO();
  while(1){
    LED_ON();
    Delay(0xfffff);
    LED_OFF();
    Delay(0xfffff);
  }

  return 0;
}
