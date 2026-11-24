.syntax unified
.cpu cortex-m3
.thumb

.section .text.context,"ax",%progbits
.global start_kernel
.type start_kernel, %function
.thumb_func
start_kernel:
    /* PSP=0 marks the first launch, which has no outgoing task. */
    movs r0, #0
    msr psp, r0
    ldr r0, =0xE000ED04
    ldr r1, =0x04000000       /* Pend SysTick once for bootstrap. */
    str r1, [r0]
    dsb
    isb
    cpsie i
1:
    b 1b                     /* main is never resumed. */

.global SysTick_Handler
.type SysTick_Handler, %function
.thumb_func
SysTick_Handler:
    cpsid i
    mrs r0, psp
    cbz r0, first_task
    stmdb r0!, {r4-r11}      /* Save registers not stacked by hardware. */
    b select_task

first_task:
    /* Discard main's frame and start with a clean handler stack. */
    ldr r1, =_estack
    msr msp, r1

select_task:
    push {r3, lr}            /* Preserve EXC_RETURN; keep MSP aligned. */
    bl tick_update           /* C chooses which saved context to restore. */
    pop {r3, lr}
    ldmia r0!, {r4-r11}
    msr psp, r0              /* PSP now points to the hardware frame. */
    ldr lr, =0xFFFFFFFD      /* Cortex-M3: return to Thread mode on PSP. */
    cpsie i
    bx lr
