#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>
#include "stabilizer_types.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SATURATE(out, limit) (out > 0) ? MIN(limit, out) : MAX(-limit, out)
#define CONSTRAIN(out, limitL, limitH) (out > limitH) ? limitH : (out < limitL ? limitL : out)

#define PID_PITCH_RATE_KP 1
#define PID_PITCH_RATE_KI 0
#define PID_PITCH_RATE_KD 0
#define PID_PITCH_RATE_INTEGRAL_LIMIT 0
#define PID_PITCH_RATE_INTEGRAL_RANGE 0

#define PID_PITCH_KP 1
#define PID_PITCH_KI 0
#define PID_PITCH_KD 0
#define PID_PITCH_INTEGRAL_LIMIT 0
#define PID_PITCH_INTEGRAL_RANGE 5 // 5 degrees

#define PID_YAW_RATE_KP 0
#define PID_YAW_RATE_KI 0
#define PID_YAW_RATE_KD 0
#define PID_YAW_RATE_INTEGRAL_LIMIT 0
#define PID_YAW_RATE_INTEGRAL_RANGE 0

#define PID_YAW_KP 0
#define PID_YAW_KI 0
#define PID_YAW_KD 0
#define PID_YAW_INTEGRAL_LIMIT 0
#define PID_YAW_INTEGRAL_RANGE 5

#define PID_VELOCITY_KP 0
#define PID_VELOCITY_KI 0
#define PID_VELOCITY_KD 0
#define PID_VELOCITY_INTEGRAL_LIMIT 0
#define PID_VELOCITY_INTEGRAL_RANGE 0

typedef struct {
    float error;     // state error
    float prevError; // error at previous time instant
    float integral;  // integral term
    float derivative;// derivative term
    float prevDeriv; // previous derivative
    float reference; // reference input
    float iLimit;    // integral limit
    float iRange;    // work range of integral
    float dt;        // delta t
    float kp;        // propotional
    float ki;        // integral
    float kd;        // derivative
} control_t;

void pidInit(control_t *pid, const float ref, const float dt, const float iLimit,
        const float iRange, const float kp, const float ki, const float kd);
void controllerInit(void);
bool controllerTest(void);
void commanderGetSetpoint(setpoint_t *sp);
float pidUpdate(control_t *pid, const float measurement, const bool updateError);
void stateController(mixer_t *mixer, const sensorData_t *sensors, const state_t *state,
        const setpoint_t *sp, const uint32_t tick);

#endif
