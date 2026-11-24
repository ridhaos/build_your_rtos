ENTRY(Reset_Handler)

/* This is the QEMU AN385 map, not a universal Cortex-M memory map.
   CODE is backed by emulated RAM but used as our firmware image. */
MEMORY
{
    CODE (rx)  : ORIGIN = 0x00000000, LENGTH = 4M
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 4M
}
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Keep the reset vector at the start of the image. */
    .isr_vector : { KEEP(*(.isr_vector)) } > CODE
    .text : {
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > CODE

    /* Writable data lives in RAM; its initial bytes are in CODE. */
    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > CODE
    _sidata = LOADADDR(.data);

    /* No initialized bytes are needed for zero-initialized storage. */
    .bss (NOLOAD) : {
        . = ALIGN(8);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        _ebss = .;
    } > RAM

    ASSERT(_ebss <= _estack - 4096, "Leave at least 4 KiB for MSP")
}
