#include "sched.h"
#include "function.h"
#include "irq.h"

//void disable_irq(void);
//void enable_irq(void);
//void cpu_switch_to(struct task_struct* prev, struct task_struct* next);
static struct task_struct init_task = INIT_TASK;
struct task_struct *current = &(init_task);
struct task_struct * task[NR_TASKS] = {&(init_task), };
int nr_tasks = 1;
int timer_count=0;

void __print_sched(int i) 
{
	uart_puts("[-] Switch to task with pid: ");
	uart_putx(task[i]->pid);
        

	#ifdef non_priority
	uart_puts(", prio: ");
	uart_putx(task[i]->priority);
	#else
	uart_puts(", add_prio: ");
	uart_putx(task[i]->add_priority); 
	#endif

	uart_puts(", counter: ");
	uart_putx(task[i]->counter);
	uart_puts(", task_struct's va: ");
        uart_putx((unsigned long)(task[i]));
	uart_puts("\n");
}

#ifdef non_priority
#else
void __print_change(int i, int p)
{
	uart_puts("[*] task ");
	uart_putx(i);
	uart_puts(" change priority from ");
	uart_putx(task[i]->add_priority);
	uart_puts(" to ");
	uart_putx(p);
	uart_puts("\n");
	task[i]->add_priority=p;
}
#endif

void preempt_disable(void)
{
	current->preempt_count++;
}

void preempt_enable(void)
{
	current->preempt_count--;
}


void schedule(void)
{
	uart_puts("[^] Enter `schedule` function at ");
   	uart_putx((unsigned int)(unsigned long)&schedule);
   	uart_puts("\n");
	int next,c,prio;
	struct task_struct * p;
	while (1) {
		c = -1;
		next = 0;
		prio=-1;
		int i =0;
		for(i=0;task[i];i++){};
		int k=i-1;
		for (int i=k ; i>0 ; i--){
			p = task[i];
			#ifdef non_priority
			if(p&&p->counter>c){
				c = p->counter;
				next = i;
			}
			#else
			if(p && p->counter > 0 && p->add_priority >prio){
				prio = p->add_priority;
				c = p->counter;
				next = i;
			}
			else if (p &&p->counter > c&& p->add_priority == prio) {
				c = p->counter;
				next = i;
			}
			#endif
		}
		if(c==-1) c = 0;
		if(c) break;
		for (int i=k ; i>=0 ; i--) {
			p = task[i];
			if (p) {
				p->counter = (p->counter >> 1) + p->priority;
			}
		}
	}
	
	__print_sched(next);
	switch_to(task[next]);
}

void switch_to(struct task_struct * next) 
{
	uart_puts("[^] Enter `switch_to` function at ");
    	uart_putx((unsigned long)&switch_to);
    	uart_puts("\n");
    	uart_puts("\n");
	if (current == next){
		return;
	}
		
	struct task_struct* prev = current;
	current = next;
	cpu_switch_to(prev, next);
	return;
}

void schedule_tail(void) {
	preempt_enable();
}


void do_timer()
{	
	--current->counter;
	#ifdef non_priority
	if (current->counter>0 || current->preempt_count >0) {
		return;
	}
	current->counter=0;
	#else
	timer_count++;
	if (timer_count==4)
	{
		__print_change(4,3);// change task add_priority
	}
	if (timer_count==9)
	{
		__print_change(4,2);
	}
	#endif
	enable_irq();
	schedule();
        disable_irq();
	
}

