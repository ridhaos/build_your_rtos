.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler

.extern main

.extern PendSV_Handler
.extern SysTick_Handler

.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss


/*
 * Cortex-M vector table.
 */

.section .isr_vector, "a", %progbits

.word _estack
.word Reset_Handler

.word Default_Handler       /* NMI */
.word Default_Handler       /* HardFault */
.word Default_Handler       /* MemManage */
.word Default_Handler       /* BusFault */
.word Default_Handler       /* UsageFault */

.word 0
.word 0
.word 0
.word 0

.word Default_Handler       /* SVCall */
.word Default_Handler       /* DebugMon */

.word 0

.word PendSV_Handler        /* PendSV */
.word SysTick_Handler       /* SysTick */


/*
 * Reset handler.
 */

.section .text.Reset_Handler, "ax", %progbits

.thumb_func

Reset_Handler:

    /*
     * Initialize main stack pointer.
     */

    ldr r0, =_estack
    mov sp, r0

    /*
     * Copy initialized data from Flash to RAM.
     */

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:

    cmp r1, r2
    bcs zero_bss

    ldr r3, [r0], #4
    str r3, [r1], #4

    b copy_data


/*
 * Clear the BSS section.
 */

zero_bss:

    ldr r1, =_sbss
    ldr r2, =_ebss

clear_bss:

    cmp r1, r2
    bcs call_main

    movs r3, #0
    str r3, [r1], #4

    b clear_bss


/*
 * Enter the C application.
 */

call_main:

    bl main


/*
 * main() should never return.
 */

hang:

    b hang


/*
 * Default exception handler.
 */

.section .text.Default_Handler, "ax", %progbits

.thumb_func

Default_Handler:

    b Default_Handler
