CC := arm-none-eabi-gcc

# Build bare-metal Thumb code with symbols for the debugger.
CFLAGS := -mcpu=cortex-m3 -mthumb -std=c11 -O1 -g3
CFLAGS += -ffreestanding -fno-builtin -Wall -Wextra

# Supply our own startup and linker layout; no C runtime library.
LDFLAGS := -nostdlib -T linker.ld -Wl,--build-id=none

QEMU := qemu-system-arm -M mps2-an385 \
        -display none -monitor none -serial stdio

.PHONY: all run debug clean

all: rtos.elf

rtos.elf: main.c context.S startup.S linker.ld Makefile
	$(CC) $(CFLAGS) main.c context.S startup.S $(LDFLAGS) -o $@

run: rtos.elf
	$(QEMU) -kernel $<

debug: rtos.elf
	$(QEMU) -kernel $< -S -gdb tcp:127.0.0.1:1234

clean:
	rm -f rtos.elf
