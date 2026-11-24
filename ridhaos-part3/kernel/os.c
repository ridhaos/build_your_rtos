#include "os.h"


/*
 * Initialize the kernel.
 */

void os_init(void)
{
    uart_init();

    task_system_init();

    scheduler_init();

    systick_init();

    printf("\r\n");
    printf("[KERNEL] Initialization complete\r\n");
    printf("[KERNEL] CPU frequency: %lu Hz\r\n",
           (unsigned long)OS_CPU_HZ);

    printf("[KERNEL] Tick frequency: %lu Hz\r\n",
           (unsigned long)OS_TICK_HZ);

    printf("[KERNEL] Time slice: %lu ticks\r\n",
           (unsigned long)OS_TIME_SLICE_TICKS);
}


/*
 * Start the kernel.
 */

void os_start(void)
{
    if (os_task_head == 0)
    {
        printf("[KERNEL] ERROR: No tasks created\r\n");
        return;
    }

    printf("[KERNEL] Starting scheduler...\r\n");

    /*
     * This function does not return.
     *
     * It requests the first PendSV exception.
     */

    start_kernel();
}
