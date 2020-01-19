// Memory-Mapped I/O output
static void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static uint64_t mmio_read(uint64_t reg)
{
	return *(volatile uint64_t*)reg;
}
// Change int to Hex String and print
static void uart_printHex(uint32_t num)
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

static void uart_putc(unsigned char c)
{
	// Wait for UART to become ready to transmit.
	while ( mmio_read(UART0_FR) & (1 << 5) ) { }

	mmio_write(UART0_DR, c);
}
 
static unsigned char uart_getc()
{
    // Wait for UART to have received something.
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}
 
static void uart_puts(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++)
		uart_putc((unsigned char)str[i]);
}

