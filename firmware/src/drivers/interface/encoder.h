#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include "stabilizer_types.h"

typedef enum {
    LEFT,
    RIGHT
} EncoderIdx;

void encoderInit(void);
int16_t encoderRead(EncoderIdx);

void encoderAcquire(encoderData_t *encoders, uint32_t tick);

#endif
