#include <stddef.h>
#include <stdint.h>
  
// Memory-Mapped I/O output
static inline void mmio_write(uint64_t reg, uint64_t data)
{
	*(volatile uint64_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint64_t mmio_read(uint64_t reg)
{
	return *(volatile uint64_t*)reg;
}
// The base address for UART.
#define	UART0_BASE 0x3F201000// for raspi2 & 3
 
// The offsets for reach register for the UART.
#define	UART0_DR     (UART0_BASE + 0x00)
#define	UART0_FR     (UART0_BASE + 0x18)
 
void uart_putc(unsigned char c)
{
	// Wait for UART to become ready to transmit.
	while ( mmio_read(UART0_FR) & (1 << 5) ) { }
	mmio_write(UART0_DR, c);
}
 
unsigned char uart_getc()
{
    // Wait for UART to have received something.
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}
 
void uart_puts(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++)
		uart_putc((unsigned char)str[i]);
}

void kernel_main(uint64_t r0, uint64_t r1, uint64_t atags)
{
	// Declare as unused
	(void) r0;
	(void) r1;
	(void) atags;

	uart_puts("hello ARM64\r\n");
 
	while (1)
		uart_putc(uart_getc());
}
