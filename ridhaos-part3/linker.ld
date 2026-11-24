ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 4M
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 4M
}

/*
 * Initial stack pointer.
 */

_estack = ORIGIN(RAM) + LENGTH(RAM);


SECTIONS
{
    /*
     * Interrupt vector table.
     */

    .isr_vector :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

    } > FLASH


    /*
     * Program code and read-only data.
     */

    .text :
    {
        . = ALIGN(4);

        *(.text*)
        *(.rodata*)

        . = ALIGN(4);

    } > FLASH


    /*
     * Initialized data.
     *
     * Stored in Flash and copied to RAM at startup.
     */

    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data*)

        . = ALIGN(4);

        _edata = .;

    } > RAM AT > FLASH

    _sidata = LOADADDR(.data);


    /*
     * Uninitialized data.
     */

    .bss (NOLOAD) :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;

    } > RAM


    /*
     * End of static RAM allocation.
     */

    . = ALIGN(8);

    end = .;
}
