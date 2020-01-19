#include "function.h"
#include "timer.h"
#include "mm.h"
#include "sched.h"
#include "irq.h"
//void irq_vector_init(void);
//void enable_irq(void);
void ret_from_fork(void);

void process(void)
{
	while (1){};
}

int copy_process(unsigned long fn, long prio)
{
	struct task_struct *p;

	p = (struct task_struct *) get_free_page();
	if (!p)
		return 1;
#ifdef non_priority
#else
	p->add_priority=prio;
#endif
	p->priority = prio;
	p->counter = current->priority;
	p->tss.x19 = fn;
	p->tss.pc = (unsigned long)ret_from_fork;
	p->tss.sp = (unsigned long)p + THREAD_SIZE;
	int pid = nr_tasks++;
	task[pid] = p;
	p->pid=pid;
	return 0;
}

void task_init(void)
{
	int res;
#ifdef non_priority
	res = copy_process((unsigned long)&process,0xa);
	if (res!=0)
	{
		uart_puts("error in p1");
		return;
	}
	res = copy_process((unsigned long)&process,0x14);
	if (res!=0)
	{
		uart_puts("error in p2");
		return;
	}
	res = copy_process((unsigned long)&process,0x1e);
	if (res!=0)
	{
		uart_puts("error in p3");
		return;
	}
	res = copy_process((unsigned long)&process,0x28);
	if (res!=0)
	{
		uart_puts("error in p4");
		return;
	}

#else
	res = copy_process((unsigned long)&process,0x2);
	if (res!=0)
	{
		uart_puts("error in p1");
		return;
	}
	res = copy_process((unsigned long)&process,0x1);
	if (res!=0)
	{
		uart_puts("error in p2");
		return;
	}
	res = copy_process((unsigned long)&process,0x3);
	if (res!=0)
	{
		uart_puts("error in p3");
		return;
	}
	res = copy_process((unsigned long)&process,0x4);
	if (res!=0)
	{
		uart_puts("error in p4");
		return;
	}
#endif
}


void kernel_main(void)
{
	create_mapping();
	uart_puts("We are ARM64 Group1!\n\n");
	uart_puts("Lab4 by 3170106269 3170105477 3170103646");
	
	task_init();
	mmio_write(V_LOCAL_INTERRUPT_ROUTE,0);
	mmio_write(V_LOCAL_TIMER_CONTROL,0x32000000); //Enable Interrupt and Timer,and set Re-load value
	irq_vector_init();
	enable_irq();

	while (1){
		schedule();
	}
}


