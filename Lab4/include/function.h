#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
void init_irq();
void mmio_write(uint32_t reg, uint32_t data);
uint64_t mmio_read(uint64_t reg);
void uart_putc(unsigned char c);
unsigned char uart_getc();
void uart_puts(const char* str);
void uart_printHex(uint32_t num);
void uart_putx(int i);
// The base address for UART.
#define	UART0_BASE 0x1001000// for raspi3
// The offsets for reach register for the UART.
#define	UART0_DR     (UART0_BASE + 0x00)
#define	UART0_FR     (UART0_BASE + 0x18)

#define	LOCAL_INTERRUPT_ROUTING	0x40000024
#define CORE0_INTERRUPT_SOURCE 0x40000060
#define LOCAL_TIMER_CONTROL 0x40000034
#define LOCAL_TIMER_CLEAR 0x40000038


