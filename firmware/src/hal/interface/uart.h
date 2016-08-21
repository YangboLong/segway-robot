#ifndef __UART_H__
#define __UART_H__

#ifdef PLATFORM_F4
#include "stm32f4xx.h"
#endif
#include "eprintf.h"

#define UART_TYPE       USART3

int uartPutchar(int ch);
#define DEBUG_PRINTF(FMT, ...) eprintf(uartPutchar, FMT, ## __VA_ARGS__)

void uartInit(void);

#endif
