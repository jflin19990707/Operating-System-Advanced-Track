#include "timer.h"
#include "function.h"
#include "function.c"


void irq_main()
{
	if(mmio_read(CORE0_INTERRUPT_SOURCE)&1<<11)
	{
		if(mmio_read(LOCAL_TIMER_CONTROL) & 1<<31)
		{
			timer_main();
		}
	}
}


