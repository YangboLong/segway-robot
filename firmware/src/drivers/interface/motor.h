#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdbool.h>
#include "stm32f429i_discovery.h"

void motorInit(void);
void powerDistribution(void);
bool motorTest(void);

#endif
