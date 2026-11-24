#include <stdint.h>

#include "os.h"


/*
 * Simple delay used only for demonstration.
 *
 * This is not an RTOS delay function.
 * We will implement proper task delays in Part 4.
 */

static void busy_delay(void)
{
    for (volatile uint32_t i = 0; i < 200000UL; i++)
    {
    }
}


/*
 * Task A.
 */

static void task_a(void)
{
    uint32_t counter = 0;

    while (1)
    {
        printf("[TASK A] counter=%lu tick=%lu\r\n",
               (unsigned long)counter++,
               (unsigned long)os_ticks);

        busy_delay();
    }
}


/*
 * Task B.
 */

static void task_b(void)
{
    uint32_t counter = 0;

    while (1)
    {
        printf("[TASK B] counter=%lu tick=%lu\r\n",
               (unsigned long)counter++,
               (unsigned long)os_ticks);

        busy_delay();
    }
}


/*
 * Application entry point.
 */

int main(void)
{
    os_init();

    printf("[APP] Creating tasks...\r\n");

    if (os_task_create(task_a) != 0)
    {
        printf("[APP] ERROR: Could not create Task A\r\n");

        while (1)
        {
        }
    }

    if (os_task_create(task_b) != 0)
    {
        printf("[APP] ERROR: Could not create Task B\r\n");

        while (1)
        {
        }
    }

    printf("[APP] Task A created\r\n");
    printf("[APP] Task B created\r\n");

    os_start();

    while (1)
    {
    }
}
