.syntax unified
.cpu cortex-m3
.thumb

.global start_kernel
.global PendSV_Handler

.extern scheduler_select_next
.extern systick_start

.extern _estack


/*
 * Start the kernel.
 *
 * PendSV is requested to launch the first task.
 */

.type start_kernel, %function

start_kernel:

    /*
     * Make sure interrupts are disabled while
     * preparing the first task launch.
     */

    cpsid i

    /*
     * PSP = 0 identifies the first launch.
     *
     * Thread mode currently uses MSP.
     */

    movs r0, #0
    msr psp, r0

    /*
     * Request PendSV.
     *
     * ICSR.PENDSVSET is bit 28.
     */

    ldr r0, =0xE000ED04
    ldr r1, =0x10000000
    str r1, [r0]

    /*
     * Enable interrupts.
     *
     * The pending PendSV exception will now execute.
     */

    cpsie i

1:
    b 1b


/*
 * PendSV context-switch handler.
 */

.type PendSV_Handler, %function

PendSV_Handler:

    /*
     * Read the current PSP.
     */

    mrs r0, psp

    /*
     * First launch?
     *
     * At startup, no task is running and PSP is zero.
     */

    cbz r0, first_task_launch


    /*
     * Normal context switch.
     *
     * Save R4-R11 on the outgoing task stack.
     */

    stmdb r0!, {r4-r11}

    b select_next_task


first_task_launch:

    /*
     * The first PendSV exception was entered from
     * Thread mode using MSP.
     *
     * We are abandoning that initial Thread-mode
     * exception frame because we will return using PSP.
     *
     * Reset MSP to the top of RAM before calling C.
     */

    ldr r1, =_estack
    msr msp, r1

    /*
     * No outgoing task exists.
     */

    movs r0, #0

    /*
     * Start SysTick now that the task list is ready
     * and the first task is about to run.
     */

    push {r3, lr}

    bl systick_start

    pop {r3, lr}


select_next_task:

    /*
     * Preserve LR while calling the C scheduler.
     *
     * LR contains the exception-return value.
     */

    push {r3, lr}

    /*
     * Input:
     * R0 = outgoing task stack pointer.
     *
     * Output:
     * R0 = incoming task stack pointer.
     */

    bl scheduler_select_next

    /*
     * R1 = stack pointer of the next task.
     */

    mov r1, r0

    /*
     * Restore the exception-return value.
     */

    pop {r3, lr}


    /*
     * Restore R4-R11 from the incoming task stack.
     */

    ldmia r1!, {r4-r11}

    /*
     * Update PSP.
     *
     * PSP now points to the hardware exception frame.
     */

    msr psp, r1

    /*
     * Return to Thread mode using PSP.
     *
     * Cortex-M3 basic exception frame:
     * 0xFFFFFFFD
     */

    ldr lr, =0xFFFFFFFD

    bx lr
