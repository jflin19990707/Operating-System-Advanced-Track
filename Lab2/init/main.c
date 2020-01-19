#include "function.h"
#include "function.c"
#include "timer.h"
//extern void init_irq();
void kernel_main(uint64_t r0, uint64_t r1, uint64_t atags)
{
	// Declare as unused
	(void) r0;
	(void) r1;
	(void) atags;
	//uart_puts("main\n");
	mmio_write(LOCAL_INTERRUPT_ROUTING,0);
	mmio_write(LOCAL_TIMER_CONTROL,0x32000000); //Enable Interrupt and Timer,and set Re-load value
	//mmio_write(CORE0_INTERRUPT_SOURCE, 1 << 11);
	init_irq();
	while (1);
}

