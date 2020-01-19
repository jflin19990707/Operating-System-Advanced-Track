#include "function.h"
// Memory-Mapped I/O output
void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
uint64_t mmio_read(uint64_t reg)
{
	return *(volatile uint64_t*)reg;
}

void uart_printHex(uint32_t num)
{
	uint32_t temp;
	uint32_t target = 0;
	unsigned char resSequence[100];
	unsigned char CharSequence[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	do{
		target++;
		temp = num%16;
		num = num/16;
		resSequence[target++] = CharSequence[temp];
	}while(num!=0);

	for(uint32_t i = target - 1; i > 0; i--){
		uart_putc(resSequence[i]);
	}
}

void uart_putx(int i)
{
	int digit;
	digit=1;
	char c;
	
	uart_puts("0x");

	if (i==0) uart_putc('0');

	while (i/digit>0)
	{
		digit*=16;
	}
	digit/=16;

	while (digit>=1)
	{
		if (i/digit>=10) 
		{
			c=i/digit-10+'a';
		}
		else
			c=i/digit+'0';
		i=i%digit;
		uart_putc(c);
		digit/=16;
	}
}
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

