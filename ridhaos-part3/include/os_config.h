#ifndef OS_CONFIG_H
#define OS_CONFIG_H

/*
 * Cortex M3
 */

#define OS_CPU_HZ              25000000UL
#define OS_TICK_HZ             1000UL

#define OS_TIME_SLICE_TICKS    10UL

#define OS_MAX_TASKS           4U
#define OS_STACK_WORDS         256U

#define OS_UART_BASE           0x40004000UL
#define OS_UART_BAUDDIV        16UL

/*
 * Compile-time configuration checks.
 */

#if (OS_CPU_HZ % OS_TICK_HZ) != 0
#error "OS_CPU_HZ must be divisible by OS_TICK_HZ"
#endif

#if ((OS_CPU_HZ / OS_TICK_HZ) - 1UL) > 0xFFFFFFUL
#error "SysTick reload value exceeds 24-bit limit"
#endif

#if OS_MAX_TASKS < 1
#error "At least one task must be configured"
#endif

#if OS_STACK_WORDS < 64
#error "Task stacks are too small"
#endif

#endif
