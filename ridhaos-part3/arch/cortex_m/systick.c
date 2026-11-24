#include <stdint.h>

#include "os.h"

/*
 * Cortex-M system control registers.
 */

#define SYST_CSR    (*(volatile uint32_t *)0xE000E010UL)
#define SYST_RVR    (*(volatile uint32_t *)0xE000E014UL)
#define SYST_CVR    (*(volatile uint32_t *)0xE000E018UL)

#define SCB_ICSR    (*(volatile uint32_t *)0xE000ED04UL)

/*
 * System Handler Priority Register 3.
 *
 * PendSV priority: byte at 0xE000ED22
 * SysTick priority: byte at 0xE000ED23
 */

#define SCB_SHPR3_PENDSV \
    (*(volatile uint8_t *)0xE000ED22UL)

#define SCB_SHPR3_SYSTICK \
    (*(volatile uint8_t *)0xE000ED23UL)


/*
 * SysTick control bits.
 */

#define SYST_CSR_ENABLE       (1UL << 0)
#define SYST_CSR_TICKINT      (1UL << 1)
#define SYST_CSR_CLKSOURCE    (1UL << 2)

/*
 * PendSV pending bit.
 */

#define SCB_ICSR_PENDSVSET    (1UL << 28)


/*
 * Configure SysTick.
 *
 * The timer is configured but not started yet.
 * It will start when the first task is launched.
 */

void systick_init(void)
{
    uint32_t reload =
        (OS_CPU_HZ / OS_TICK_HZ) - 1UL;

    /*
     * Disable SysTick during configuration.
     */

    SYST_CSR = 0;

    /*
     * Configure reload value.
     */

    SYST_RVR = reload;

    /*
     * Clear current counter value.
     */

    SYST_CVR = 0;

    /*
     * PendSV gets the lowest configurable priority.
     */

    SCB_SHPR3_PENDSV = 0xFF;

    /*
     * SysTick has a higher priority than PendSV.
     */

    SCB_SHPR3_SYSTICK = 0x80;
}


/*
 * Start SysTick.
 *
 * This is called during the first PendSV launch.
 */

void systick_start(void)
{
    SYST_CSR =
        SYST_CSR_ENABLE |
        SYST_CSR_TICKINT |
        SYST_CSR_CLKSOURCE;
}


/*
 * SysTick exception handler.
 */

void SysTick_Handler(void)
{
    /*
     * Update the system tick.
     *
     * If the time slice has expired, request PendSV.
     */

    if (scheduler_on_tick())
    {
        SCB_ICSR = SCB_ICSR_PENDSVSET;
    }
}
