#include "stabilizer.h"
#include "stabilizer_types.h"
#include "estimator.h"
#include "sensfusion6.h"
#include "uart.h"

#define ATTITUDE_UPDATE_RATE RATE_250_HZ
#define ATTITUDE_UPDATE_DT 1.0/ATTITUDE_UPDATE_RATE

void stateEstimatorInit(void)
{
    sensfusion6Init();
}

bool stateEstimatorTest(void)
{
    bool pass = true;

    pass &= sensfusion6Test();

    return pass;
}

void stateEstimator(state_t *state, sensorData_t *sensorData, const uint32_t tick)
{
    if (RATE_DO_EXECUTE(ATTITUDE_UPDATE_RATE, tick)) {
        sensfusion6UpdateQ(sensorData->gyro.x, sensorData->gyro.y, sensorData->gyro.z,
                           sensorData->acc.x, sensorData->acc.y, sensorData->acc.z,
                           ATTITUDE_UPDATE_DT);
        sensfusion6GetEulerRPY(&state->attitude.roll, &state->attitude.pitch, &state->attitude.yaw);

        // Coordinate transformation
        coordinateTrans(state, sensorData);
    }
    if(RATE_DO_EXECUTE(1, tick)) {
        DEBUG_PRINTF("(roll: %f, pitch: %f, yaw: %f). ",
                state->attitude.roll, state->attitude.pitch, state->attitude.yaw);
    }
}

void coordinateTrans(state_t *state, sensorData_t *sensorData)
{
    float tmp = -state->attitude.roll;
    state->attitude.roll = state->attitude.pitch;
    state->attitude.pitch = tmp;
    state->attitude.yaw *= -1;

    tmp = -sensorData->gyro.x;
    sensorData->gyro.x = sensorData->gyro.y;
    sensorData->gyro.y = tmp;
    sensorData->gyro.z *= -1;
}

