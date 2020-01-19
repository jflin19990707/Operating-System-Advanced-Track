#include "timer.h"
#include "function.h"
#include "mm.h"


void irq_main()
{
	if(mmio_read(V_INTERRUPT_SOURCE_0)&1<<11)
	{
		if(mmio_read(V_LOCAL_TIMER_CONTROL) & 1<<31)
		{
			timer_main();
		}
	}
}


