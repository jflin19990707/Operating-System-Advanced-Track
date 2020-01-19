#include "function.h"
#include "sched.h"
#include "mm.h"
static int count = 0;
void timer_main()
{
//	uart_puts("Timer Interrupt Received~~ count = ");
//	uart_printHex(count);
//	uart_puts("\n");
//	count = count + 1;
	mmio_write(V_LOCAL_TIMER_CLEAR,1<<31);
	do_timer();
}

