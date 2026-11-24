CROSS_COMPILE ?= arm-none-eabi-

CC      := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
SIZE    := $(CROSS_COMPILE)size

QEMU ?= qemu-system-arm

TARGET := build/rtos.elf

C_SOURCES := \
    kernel/os.c \
    kernel/scheduler.c \
    kernel/task.c \
    arch/cortex_m/systick.c \
    arch/cortex_m/uart.c \
    app/main.c

ASM_SOURCES := \
    startup.S \
    arch/cortex_m/context.S

OBJECTS := \
    $(patsubst %.c,build/%.o,$(C_SOURCES)) \
    $(patsubst %.S,build/%.o,$(ASM_SOURCES))


CFLAGS := \
    -mcpu=cortex-m3 \
    -mthumb \
    -std=c11 \
    -O1 \
    -g3 \
    -ffreestanding \
    -fno-builtin \
    -fno-common \
    -fdata-sections \
    -ffunction-sections \
    -fno-unwind-tables \
    -fno-asynchronous-unwind-tables \
    -Wall \
    -Wextra \
    -Werror \
    -Iinclude

ASFLAGS := \
    -mcpu=cortex-m3 \
    -mthumb \
    -g3 \
    -Iinclude

LDFLAGS := \
    -mcpu=cortex-m3 \
    -mthumb \
    -nostdlib \
    -Wl,--gc-sections \
    -Wl,-Map=build/rtos.map \
    -T linker.ld


.PHONY: all clean run size

all: $(TARGET)


$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)

	$(CC) $(LDFLAGS) $(OBJECTS) -lgcc -o $@

	$(SIZE) $@


build/%.o: %.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@


build/%.o: %.S
	@mkdir -p $(dir $@)

	$(CC) $(ASFLAGS) -c $< -o $@


run: $(TARGET)
	$(QEMU) \
	    -M mps2-an385 \
	    -cpu cortex-m3 \
	    -display none \
	    -serial stdio \
	    -monitor none \
	    -kernel $(TARGET)


size: $(TARGET)
	$(SIZE) $(TARGET)


clean:
	rm -rf build
