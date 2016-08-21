#ifndef CONFIG_H
#define CONFIG_H

#define FREERTOS_HEAP_SIZE        22000
#define FREERTOS_MIN_STACK_SIZE   150 // M4-FPU register setup is bigger so stack needs to be bigger
#define FREERTOS_MCU_CLOCK_HZ     180000000

// Task priorities. Higher number higher priority
#define STABILIZER_TASK_PRI       4
#define SYSTEM_TASK_PRI           2
#define UART_RX_TASK_PRI          2

// Task names
#define SYSTEM_TASK_NAME          "SYSTEM"
#define STABILIZER_TASK_NAME      "STABILIZER"
#define UART_RX_TASK_NAME         "UART"

// Task stack sizes
#define SYSTEM_TASK_STACKSIZE     (2 * configMINIMAL_STACK_SIZE)
#define STABILIZER_TASK_STACKSIZE (3 * configMINIMAL_STACK_SIZE)
#define UART_RX_TASK_STACKSIZE    configMINIMAL_STACK_SIZE

#endif
