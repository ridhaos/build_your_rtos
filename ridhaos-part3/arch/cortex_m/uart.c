#include <stdint.h>
#include <stdarg.h>

#include "os.h"


#define UART_DATA \
    (*(volatile uint32_t *)(OS_UART_BASE + 0x00UL))

#define UART_STATE \
    (*(volatile uint32_t *)(OS_UART_BASE + 0x04UL))

#define UART_CTRL \
    (*(volatile uint32_t *)(OS_UART_BASE + 0x08UL))

#define UART_BAUDDIV \
    (*(volatile uint32_t *)(OS_UART_BASE + 0x10UL))


#define UART_STATE_TXFULL    (1UL << 0)


/*
 * Save and disable interrupts.
 *
 * This prevents a task switch in the middle of
 * a printf operation, which would interleave output.
 */

static uint32_t irq_save(void)
{
    uint32_t primask;

    __asm volatile
    (
        "mrs %0, primask\n"
        "cpsid i\n"
        : "=r" (primask)
        :
        : "memory"
    );

    return primask;
}


/*
 * Restore the previous interrupt state.
 */

static void irq_restore(uint32_t primask)
{
    __asm volatile
    (
        "msr primask, %0\n"
        :
        : "r" (primask)
        : "memory"
    );
}


/*
 * Initialize UART.
 */

void uart_init(void)
{
    UART_CTRL = 0;

    UART_BAUDDIV = OS_UART_BAUDDIV;

    /*
     * Enable UART transmitter.
     */

    UART_CTRL = 1;
}


/*
 * Transmit one character.
 */

static void uart_putc(char c)
{
    /*
     * Wait while the transmit FIFO is full.
     */

    while (UART_STATE & UART_STATE_TXFULL)
    {
    }

    UART_DATA = (uint32_t)c;
}


/*
 * Convert an unsigned integer to a string.
 */

static void print_unsigned(unsigned long value,
                            unsigned int base)
{
    char buffer[16];

    unsigned int index = 0;

    if (value == 0)
    {
        uart_putc('0');
        return;
    }

    while (value != 0)
    {
        unsigned int digit = value % base;

        if (digit < 10)
        {
            buffer[index++] = '0' + digit;
        }
        else
        {
            buffer[index++] = 'A' + (digit - 10);
        }

        value /= base;
    }

    while (index > 0)
    {
        uart_putc(buffer[--index]);
    }
}


/*
 * Minimal printf implementation.
 */

int printf(const char *format, ...)
{
    va_list args;

    int count = 0;

    uint32_t primask = irq_save();

    va_start(args, format);

    while (*format != '\0')
    {
        if (*format != '%')
        {
            uart_putc(*format++);
            count++;
            continue;
        }

        format++;

        if (*format == '%')
        {
            uart_putc('%');
            count++;
            format++;
            continue;
        }

        int is_long = 0;

        if (*format == 'l')
        {
            is_long = 1;
            format++;
        }

        switch (*format)
        {
            case 'c':
            {
                char c = (char)va_arg(args, int);

                uart_putc(c);
                count++;

                break;
            }

            case 's':
            {
                const char *str = va_arg(args, const char *);

                if (str == 0)
                {
                    str = "(null)";
                }

                while (*str != '\0')
                {
                    uart_putc(*str++);
                    count++;
                }

                break;
            }

            case 'd':
            {
                long value;

                if (is_long)
                {
                    value = va_arg(args, long);
                }
                else
                {
                    value = va_arg(args, int);
                }

                if (value < 0)
                {
                    uart_putc('-');
                    count++;

                    unsigned long magnitude =
                        (unsigned long)(-(value + 1)) + 1UL;

                    print_unsigned(magnitude, 10);
                }
                else
                {
                    print_unsigned((unsigned long)value, 10);
                }

                break;
            }

            case 'u':
            {
                unsigned long value;

                if (is_long)
                {
                    value = va_arg(args, unsigned long);
                }
                else
                {
                    value = va_arg(args, unsigned int);
                }

                print_unsigned(value, 10);

                break;
            }

            case 'x':
            {
                unsigned long value;

                if (is_long)
                {
                    value = va_arg(args, unsigned long);
                }
                else
                {
                    value = va_arg(args, unsigned int);
                }

                print_unsigned(value, 16);

                break;
            }

            default:
            {
                uart_putc('%');
                uart_putc(*format);

                count += 2;

                break;
            }
        }

        format++;
    }

    va_end(args);

    irq_restore(primask);

    return count;
}
