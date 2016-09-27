#include <stdbool.h>

/* FreeRtos includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "system.h"
#include "config.h"
#include "led.h"
#include "lcd.h"
#include "uart.h"
#include "stabilizer.h"

/* Private variable */
static bool selftestPassed;
static bool isInit;

/* System wide synchronisation */
xSemaphoreHandle canStartMutex;

/* Private functions */
static void systemTask(void *arg);

void systemLaunch(void)
{
    xTaskCreate(systemTask, SYSTEM_TASK_NAME,
                SYSTEM_TASK_STACKSIZE, NULL,
                SYSTEM_TASK_PRI, NULL);
}

/* Private functions implementation */
static void systemTask(void *arg)
{
    bool pass = true;

    canStartMutex = xSemaphoreCreateMutex();
    xSemaphoreTake(canStartMutex, portMAX_DELAY);

    ledInit();
    lcdInit();

    // Initialize UART3 for printing debug info
    uartInit();

    stabilizerInit();

    isInit = true; // finished initializations

    DEBUG_PRINTF("System initialization is complete.\n");

    pass &= stabilizerTest();

    if(pass) {
        selftestPassed = 1;
        xSemaphoreGive(canStartMutex); // finished self tests
        ledSet(LED_GREEN, 1); // turn green led on
        DEBUG_PRINTF("System self-test is complete.\n");
    }
    else {
        selftestPassed = 0;
        ledSet(LED_GREEN, 0); // turn green led off
        ledSet(LED_RED, 1);   // turn red led on
        DEBUG_PRINTF("System self-test failed.\n");
    }

    int cnt = 0;
    for(;;) {
        cnt++;
        DEBUG_PRINTF("System task executed %d times. ", cnt);
        vTaskDelay(1000);
    }
}

void systemWaitStart(void)
{
    // Finish initializations and self tests before executing any other tasks.
    while(!isInit)  vTaskDelay(2);

    xSemaphoreTake(canStartMutex, portMAX_DELAY);
    xSemaphoreGive(canStartMutex);
}
