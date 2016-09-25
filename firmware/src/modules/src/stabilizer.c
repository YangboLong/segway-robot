#include <math.h>

#include "FreeRTOS.h"
#include "task.h"

#include "sensors.h"
#include "system.h"
#include "stabilizer.h"
#include "estimator.h"
#include "motor.h"
#include "encoder.h"
#include "uart.h"

static bool isInit;

// State variables for the stabilizer
// static setpoint_t setpoint;
static sensorData_t sensorData;
static state_t state;
static encoderData_t encoderData;

static void stabilizerTask(void* param);

void stabilizerInit(void)
{
    if(isInit)  return;

    sensorsInit();
    stateEstimatorInit();
    // stateControllerInit();
    motorInit();

    xTaskCreate(stabilizerTask, STABILIZER_TASK_NAME,
                STABILIZER_TASK_STACKSIZE, NULL, STABILIZER_TASK_PRI, NULL);

    isInit = true;
}

bool stabilizerTest(void)
{
    bool pass = true;

    pass &= sensorsTest();
    pass &= stateEstimatorTest();
    // pass &= stateControllerTest();
    // pass &= powerDistributionTest();

    return pass;
}

/* The stabilizer loop runs at 1kHz. It is the responsability or the different
 * functions to run slower by skipping call (ie. returning without modifying
 * the output structure).
 */
static void stabilizerTask(void* param)
{
    uint32_t tick = 0;
    uint32_t lastWakeTime;
    vTaskSetApplicationTaskTag(0, (void*)TASK_STABILIZER_ID_NBR);

    // Wait for the system to be fully started to start stabilization loop
    systemWaitStart();

    // Wait for sensors to be calibrated
    lastWakeTime = xTaskGetTickCount();
    while(!sensorsAreCalibrated()) {
        vTaskDelayUntil(&lastWakeTime, F2T(RATE_MAIN_LOOP));
    }

    while(1) {
        vTaskDelayUntil(&lastWakeTime, F2T(RATE_MAIN_LOOP));

        sensorsAcquire(&sensorData, tick);

        stateEstimator(&state, &sensorData, tick);

        encoderAcquire(&encoderData, tick);

        // commanderGetSetpoint(&setpoint, &state);
        // 
        // sitAwUpdateSetpoint(&setpoint, &sensorData, &state);
        // 
        // stateController(&control, &sensorData, &state, &setpoint, tick);
        powerDistribution();

        tick++;
    }
}
