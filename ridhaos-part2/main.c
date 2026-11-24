#include <stdint.h>

#define REG32(a) (*(volatile uint32_t *)(a))
#define REG8(a)  (*(volatile uint8_t *)(a))
#define SYST_CTRL REG32(0xE000E010u)
#define SYST_LOAD REG32(0xE000E014u)
#define SYST_VAL  REG32(0xE000E018u)

#define UART_DATA  REG32(0x40004000u)
#define UART_STATE REG32(0x40004004u)
#define UART_CTRL  REG32(0x40004008u)
#define UART_BAUD  REG32(0x40004010u)

#define CPU_HZ       25000000u
#define TICK_HZ          1000u
#define SLICE_TICKS        10u
#define STACK_WORDS       256u

typedef struct task {
    uint32_t *sp;
    struct task *next;
} task_t;

_Alignas(8) static uint32_t stacks[2][STACK_WORDS];
static task_t tasks[2];
static task_t *current_task;
volatile uint32_t os_ticks;

/* The small assembly entry point never returns to main. */
extern void start_kernel(void);

static void uart_puts(const char *text)
{
    while (*text) {
        /* Bit 0 is TXFULL: wait for room in the transmit buffer. */
        while (UART_STATE & 1u) { }
        UART_DATA = (uint32_t)(uint8_t)*text++;
    }
}

static void task_a(void)
{
    for (;;) {
        uart_puts("Task A\r\n");
        uint32_t start = os_ticks;
        /* Busy wait for readable output; this is not an RTOS sleep. */
        while ((uint32_t)(os_ticks - start) < 100u) { }
    }
}

static void task_b(void)
{
    for (;;) {
        uart_puts("Task B\r\n");
        uint32_t start = os_ticks;
        while ((uint32_t)(os_ticks - start) < 100u) { }
    }
}

static void task_return_trap(void)
{
    for (;;) { } /* Tasks must not return in this example. */
}

static void prepare_task(unsigned id, void (*entry)(void))
{
    uint32_t *sp = &stacks[id][STACK_WORDS - 16u];

    /* Eight software-saved words, followed by eight hardware words. */
    for (unsigned i = 0; i < 16u; i++) {
        sp[i] = 0;
    }
    sp[13] = (uint32_t)(uintptr_t)task_return_trap;
    sp[14] = (uint32_t)(uintptr_t)entry & ~1u;
    sp[15] = 0x01000000u; /* Required initial execution-state value. */
    tasks[id].sp = sp;
}

/* Called by the assembly SysTick handler; returns the context to restore. */
uint32_t *tick_update(uint32_t *saved_sp)
{
    static uint32_t slice_ticks;

    if (current_task == 0) {
        current_task = &tasks[0]; /* First entry launches Task A. */
        SYST_CTRL = 7u;           /* Start timer only after task setup. */
    } else {
        os_ticks++;
        current_task->sp = saved_sp;
        if (++slice_ticks == SLICE_TICKS) {
            slice_ticks = 0;
            current_task = current_task->next;
        }
    }
    return current_task->sp;
}

int main(void)
{
    /* UART0: polled transmitter, approximately 115200 baud. */
    UART_CTRL = 0;
    UART_BAUD = CPU_HZ / 115200u;
    UART_CTRL = 1u;

    prepare_task(0, task_a);
    prepare_task(1, task_b);
    tasks[0].next = &tasks[1];
    tasks[1].next = &tasks[0];

    REG32(0xE000ED14u) |= 1u << 9; /* Aligned exception frames. */
    REG8(0xE000ED23u) = 0x80u;     /* SysTick priority byte. */
    SYST_CTRL = 0;
    SYST_LOAD = CPU_HZ / TICK_HZ - 1u;
    SYST_VAL = 0;

    start_kernel();
    for (;;) { }
}
