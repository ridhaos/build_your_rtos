.syntax unified
.cpu cortex-m3
.thumb

/* Cortex-M reads its initial MSP and reset PC from this table. */
.section .isr_vector,"a",%progbits
.balign 128
.global vector_table
vector_table:
    .word _estack                 /* Entry 0: initial MSP value. */
    .word Reset_Handler           /* Entry 1: reset entry address. */
    .word Default_Handler         /* NMI. */
    .word Default_Handler         /* HardFault. */
    .word Default_Handler         /* MemManage. */
    .word Default_Handler         /* BusFault. */
    .word Default_Handler         /* UsageFault. */
    .word 0, 0, 0, 0              /* Reserved entries 7-10. */
    .word Default_Handler         /* SVC: later in the series. */
    .word Default_Handler         /* DebugMonitor. */
    .word 0                       /* Reserved entry 13. */
    .word Default_Handler
    .word SysTick_Handler         /* Entry 15. */

.section .text.Reset_Handler,"ax",%progbits
.global Reset_Handler
.type Reset_Handler, %function
.thumb_func
Reset_Handler:
    cpsid i  /* Keep interrupts masked until the kernel is ready. */
    /* Copy initialized data from its load image into writable RAM. */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
1:
    cmp r1, r2
    bcs 2f
    ldr r3, [r0], #4
    str r3, [r1], #4
    b 1b
2:
    /* The C language expects static zero-initialized storage to be zero. */
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
3:
    cmp r1, r2
    bcs 4f
    str r3, [r1], #4
    b 3b
4:
    bl main
    b Default_Handler            /* main must not return. */

.section .text.Default_Handler,"ax",%progbits
.global Default_Handler
.type Default_Handler, %function
.thumb_func
Default_Handler:
    b . 		/* Stop Here on unexpected exception */ 
