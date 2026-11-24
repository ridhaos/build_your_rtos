#ifndef OS_H
#define OS_H

#include <stdint.h>

#include "os_config.h"

/*
 * Task Control Block.
 *
 * sp:
 * Saved stack pointer for this task.
 *
 * next:
 * Pointer to the next task in the circular task list.
 */

typedef struct os_task
{
    uint32_t *sp;

    struct os_task *next;

} os_task_t;


/*
 * Global kernel variables.
 */

extern volatile uint32_t os_ticks;

extern os_task_t *os_task_head;

extern os_task_t *os_current_task;


/*
 * Kernel API.
 */

void os_init(void);

void os_start(void);

int os_task_create(void (*entry)(void));


/*
 * Task management.
 */

void task_system_init(void);

void os_task_exit(void);


/*
 * Scheduler.
 */

void scheduler_init(void);

int scheduler_on_tick(void);

uint32_t *scheduler_select_next(uint32_t *saved_sp);


/*
 * SysTick.
 */

void systick_init(void);

void systick_start(void);

void SysTick_Handler(void);


/*
 * UART and debugging.
 */

void uart_init(void);

int printf(const char *format, ...);


/*
 * Architecture-specific functions.
 */

void start_kernel(void);

void PendSV_Handler(void);

#endif
