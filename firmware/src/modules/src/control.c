#include <math.h>
#include "control.h"
#include "imu.h"
#include "encoder.h"
#include "uart.h"

#define ATTITUDE_RATE RATE_500_HZ
#define VELOCITY_RATE RATE_100_HZ

#define CUTOFF 20

static bool isInit;

control_t pidPitchRate;
control_t pidPitch;
control_t pidYawRate;
control_t pidYaw;
control_t pidVelocity;

void pidInit(control_t *pid, const float ref, const float dt, const float iLimit,
        const float iRange, const float kp, const float ki, const float kd)
{
    pid->error = 0;
    pid->prevError = 0;
    pid->integral = 0;
    pid->derivative = 0;
    pid->prevDeriv = 0;
    pid->reference = ref;
    pid->iLimit = fabs(iLimit);
    pid->iRange = fabs(iRange);
    pid->dt = fabs(dt);
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void controllerInit(void)
{
    if(isInit)  return;

    pidInit(&pidPitchRate, 0, IMU_UPDATE_DT,
            PID_PITCH_RATE_INTEGRAL_LIMIT, PID_PITCH_RATE_INTEGRAL_RANGE,
            PID_PITCH_RATE_KP, PID_PITCH_RATE_KI, PID_PITCH_RATE_KD);
    pidInit(&pidPitch, 0, IMU_UPDATE_DT,
            PID_PITCH_INTEGRAL_LIMIT, PID_PITCH_INTEGRAL_RANGE,
            PID_PITCH_KP, PID_PITCH_KI, PID_PITCH_KD);
    pidInit(&pidYawRate, 0, IMU_UPDATE_DT,
            PID_YAW_RATE_INTEGRAL_LIMIT, PID_YAW_RATE_INTEGRAL_RANGE,
            PID_YAW_RATE_KP, PID_YAW_RATE_KI, PID_YAW_RATE_KD);
    pidInit(&pidYaw, 0, IMU_UPDATE_DT,
            PID_YAW_INTEGRAL_LIMIT, PID_YAW_INTEGRAL_RANGE,
            PID_YAW_KP, PID_YAW_KI, PID_YAW_KD);
    pidInit(&pidVelocity, 0, ENCODER_UPDATE_DT,
            PID_VELOCITY_INTEGRAL_LIMIT, PID_VELOCITY_INTEGRAL_RANGE,
            PID_VELOCITY_KP, PID_VELOCITY_KI, PID_VELOCITY_KD);

    isInit = true;
}

bool controllerTest(void)
{
    return isInit;
}

void commanderGetSetpoint(setpoint_t *sp)
{
    sp->vel = 0.0;
    sp->yaw = 0.0;
}

float pidUpdate(control_t *pid, const float measurement, const bool updateError)
{
    float output;

    // propotional
    if (updateError) {
        pid->error = pid->reference - measurement;
    }

    // integral
    if (fabs(pid->error) < pid->iRange && pid->ki > 0) {
        pid->integral += pid->error * pid->dt;
        pid->integral = SATURATE(pid->integral, pid->iLimit);
    }

    // derivative
    pid->derivative = (pid->error - pid->prevError) / pid->dt;
    float rc = 1.0 / (2 * M_PI * CUTOFF);
    pid->derivative = pid->prevDeriv +
        (pid->dt / (rc + pid->dt)) * (pid->derivative - pid->prevDeriv);
    
    // update
    pid->prevError = pid->error;
    pid->prevDeriv = pid->derivative;

    output = pid->kp * pid->error + pid->ki * pid->integral + pid->kd * pid->derivative;

    return output;
}

void stateController(mixer_t *mixer, const sensorData_t *sensors, const state_t *state,
        const setpoint_t *sp, const uint32_t tick)
{
    // Pitch and yaw control, 500 hz
    if (RATE_DO_EXECUTE(ATTITUDE_RATE, tick)) {
        // Outer attitude loop PI control, compute desired pitch rate
        pidPitchRate.reference = pidUpdate(&pidPitch, state->attitude.pitch, true);
        // Inner angular rate loop PD control
        mixer->pit = pidUpdate(&pidPitchRate, sensors->gyro.y, true);

        // Yaw angle error
        pidYaw.error = sp->yaw - state->attitude.yaw;
        if (pidYaw.error > 180)   pidYaw.error -= 360;
        else if(pidYaw.error < -180)   pidYaw.error += 360;
        // Yaw angle PD control
        pidYawRate.reference = pidUpdate(&pidYaw, state->attitude.yaw, false);
        mixer->yaw = pidUpdate(&pidYawRate, sensors->gyro.z, true);
    }

    // Velocity PI control, 100 hz
    if (RATE_DO_EXECUTE(VELOCITY_RATE, tick)) {
        float vel = (sensors->encoder.left + sensors->encoder.right) / 2.0;
        pidVelocity.error = sp->vel - vel;
        mixer->vel = pidUpdate(&pidVelocity, vel, false);
    }

    // For debugging
    if (RATE_DO_EXECUTE(1, tick)) {
        DEBUG_PRINTF("mixer->pit: %f; ", mixer->pit);
        DEBUG_PRINTF("mixer->yaw: %f; ", mixer->yaw);
        DEBUG_PRINTF("mixer->vel: %f; ", mixer->vel);
    }
}

