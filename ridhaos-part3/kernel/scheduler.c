#include <stdint.h>
#include <stddef.h>
#include "os.h"

/*
 * System tick counter.
 */

volatile uint32_t os_ticks = 0;

/*
 * Currently running task.
 */

os_task_t *os_current_task = NULL;

/*
 * Number of ticks consumed by the current time slice.
 */

static uint32_t ticks_in_slice = 0;


/*
 * Initialize the scheduler.
 */

void scheduler_init(void)
{
    os_ticks = 0;

    os_current_task = NULL;

    ticks_in_slice = 0;
}


/*
 * Called from SysTick_Handler.
 *
 * Returns:
 * 1: A context switch is required.
 * 0: Continue running the current task.
 */

int scheduler_on_tick(void)
{
    os_ticks++;

    ticks_in_slice++;

    if (ticks_in_slice >= OS_TIME_SLICE_TICKS)
    {
        ticks_in_slice = 0;

        return 1;
    }

    return 0;
}


/*
 * Select the next task.
 *
 * This function is called from PendSV_Handler.
 *
 * saved_sp:
 * Stack pointer of the outgoing task.
 *
 * Return:
 * Stack pointer of the next task.
 */

uint32_t *scheduler_select_next(uint32_t *saved_sp)
{
    /*
     * If a task is already running, save its
     * updated stack pointer.
     */

    if (os_current_task != NULL)
    {
        os_current_task->sp = saved_sp;

        /*
         * Move to the next task.
         */

        os_current_task = os_current_task->next;
    }
    else
    {
        /*
         * First launch:
         * No task is currently running.
         */

        os_current_task = os_task_head;
    }

    return os_current_task->sp;
}
