# Build Your Own RTOS 🚀

**A hands-on journey into building a Real-Time Operating System from scratch on ARM Cortex-M.**

This repository documents the development of a small RTOS, starting from a simple bare-metal application and gradually introducing the fundamental concepts behind real-time operating systems.

The goal is not just to use an RTOS, but to understand how one works internally: how tasks are created, how the processor switches between them, how scheduling decisions are made, and how tasks communicate and synchronize.

We'll build everything step by step, using C wherever possible and ARM assembly only when direct processor control is required.

No commercial RTOS, no hidden implementation, and no physical development board required. We'll use QEMU to run and experiment with our kernel.

---

## 🎯 Project goals

By the end of this series, we aim to build a small but functional RTOS with:

- Preemptive multitasking.
- Round-robin task scheduling.
- Cortex-M context switching using PendSV.
- Independent task stacks and Task Control Blocks (TCBs).
- System tick and time management.
- Task delays and voluntary yielding.
- Mutexes and semaphores.
- Basic task synchronization.
- A modular kernel architecture.
- UART-based debugging and diagnostics.

We'll also explore processor-level concepts such as exception handling, stack management, interrupt priorities, and the ARM Cortex-M execution model.

## 🧭 Learning roadmap

The project is organized into independent parts. Each part introduces a new concept, explains the underlying architecture, and provides a complete implementation that can be built and executed.

| Part | Title | Main concepts |
|---|---|---|
| 1 | **First Step on the Moon** | Bare-metal limitations, RTOS fundamentals, scheduling algorithms |
| 2 | **From First Step to First Heartbeat** | SysTick, Cortex-M exception handling, task stacks, first context switch |
| 3 | **Mission Control Takes Over: Meet PendSV** | PendSV, deferred context switching, scheduler separation, modular kernel |
| 4 | **Learning to Wait: Not Every Astronaut Is Always Moving** | Task delays, periodic tasks, voluntary yielding |
| 5 | **Working Together in Space: Mutexes and Semaphores** | Synchronization, mutual exclusion, shared resources |

Further topics may include task priorities, privileged and unprivileged execution, SVC, memory protection, and Cortex-M security features.

## 📚 Articles and source code

Each part has its own directory containing the implementation developed in that article.

### Part 1: First Step on the Moon

Introduction to the RTOS world.

Topics:
- What is an RTOS?
- Why bare-metal applications become difficult to maintain.
- Tasks and scheduling.
- Round-robin scheduling.
- Rate Monotonic Scheduling (RMS).
- First architectural decisions for our kernel.

Source code: [`ridhaos-part1/`](ridhaos-part1/)

### Part 2: From First Step to First Heartbeat

Our first working multitasking implementation on Cortex-M.

Topics:
- ARM Cortex-M registers.
- MSP and PSP.
- SysTick configuration.
- Hardware exception stack frame.
- Software context preservation.
- Task Control Blocks.
- Initial task stack construction.
- First context switch.

Source code: [`ridhaos-part2/`](ridhaos-part
