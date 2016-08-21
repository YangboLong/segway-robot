/* Scheduler includes. */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "led.h"
#include "system.h"

#ifdef PLATFORM_F1

#elif defined(PLATFORM_F4)
#include "stm32f4xx.h"
#endif

int main(void)
{
    // Set up interrupts
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    // Launch the system task that will initialize and start everything
    systemLaunch();

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    ledInit();
    
    // Should never reach this point!
    while(1) {
        blinkLEDs();
    }

    return 0;
}
