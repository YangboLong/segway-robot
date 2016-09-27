#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>
#include "stabilizer_types.h"

#define ENCODER_RATE    RATE_100_HZ
#define ENCODER_UPDATE_DT   (float)(1.0 / ENCODER_RATE) 

typedef enum {
    LEFT,
    RIGHT
} EncoderIdx;

void encoderInit(void);
int16_t encoderRead(EncoderIdx);
bool encoderTest(void);

#endif
