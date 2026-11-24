#include <stdint.h>
#include <stddef.h>

#include "os.h"

/*
 * Task Control Blocks.
 */

static os_task_t os_tasks[OS_MAX_TASKS];

/*
 * Each task has its own stack.
 *
 * The stacks are aligned to 8 bytes, as required
 * by the Cortex-M exception and ABI alignment rules.
 */

static uint32_t task_stacks[OS_MAX_TASKS][OS_STACK_WORDS]
    __attribute__((aligned(8)));

os_task_t *os_task_head = NULL;

static uint32_t task_count = 0;


/*
 * Called if a task function returns.
 */

void os_task_exit(void)
{
    printf("[KERNEL] ERROR: A task returned!\r\n");

    while (1)
    {
        /*
         * A task must not return in this implementation.
         */
    }
}


/*
 * Initialize task management.
 */

void task_system_init(void)
{
    os_task_head = NULL;
    task_count = 0;
}


/*
 * Create the initial stack frame for a task.
 *
 * The stack must look as if the task had already
 * been interrupted by the processor.
 */

static uint32_t *task_stack_init(void (*entry)(void),
                                uint32_t *stack_top)
{
    uint32_t *sp = stack_top;

    /*
     * Reserve space for:
     *
     * R4-R11: 8 software-saved registers
     *
     * R0-R3, R12, LR, PC, xPSR:
     * 8 hardware-stacked registers
     */

    sp -= 16;

    /*
     * Software-saved registers.
     *
     * R4-R11.
     */

    for (uint32_t i = 0; i < 8; i++)
    {
        sp[i] = 0;
    }

    /*
     * Hardware exception frame.
     */

    sp[8]  = 0;                         /* R0    */
    sp[9]  = 0;                         /* R1    */
    sp[10] = 0;                         /* R2    */
    sp[11] = 0;                         /* R3    */
    sp[12] = 0;                         /* R12   */

    sp[13] = (uint32_t)os_task_exit;    /* LR    */

    /*
     * The stacked PC must be halfword aligned.
     * The Thumb state is indicated by xPSR.T.
     */

    sp[14] = ((uint32_t)entry) & ~1UL;  /* PC    */

    sp[15] = 0x01000000UL;              /* xPSR  */

    return sp;
}


/*
 * Add a task to the circular task list.
 */

int os_task_create(void (*entry)(void))
{
    if (entry == NULL)
    {
        return -1;
    }

    if (task_count >= OS_MAX_TASKS)
    {
        return -1;
    }

    os_task_t *task = &os_tasks[task_count];

    uint32_t *stack_top =
        &task_stacks[task_count][OS_STACK_WORDS];

    task->sp = task_stack_init(entry, stack_top);

    /*
     * First task:
     * The task points to itself.
     */

    if (os_task_head == NULL)
    {
        os_task_head = task;
        task->next = task;
    }
    else
    {
        /*
         * Find the last task in the circular list.
         */

        os_task_t *tail = os_task_head;

        while (tail->next != os_task_head)
        {
            tail = tail->next;
        }

        tail->next = task;
        task->next = os_task_head;
    }

    task_count++;

    return 0;
}
