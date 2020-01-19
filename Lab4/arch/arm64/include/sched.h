#ifndef _SCHED_H
#define _SCHED_H

#define	THREAD_CPU_CONTEXT			0 		// offset of cpu_context in task_struct 


#define THREAD_SIZE				4096

#define NR_TASKS				64 

#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS-1]

#define TASK_RUNNING				0

#define MAX_PROCESS_PAGES			16	

extern struct task_struct *current;
extern struct task_struct * task[NR_TASKS];
extern int nr_tasks;

struct tss_struct {
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
};

struct user_page {
	unsigned long phys_addr;
	unsigned long virt_addr;
};

struct mm_struct {
	unsigned long pgd;
	int user_pages_count;
	struct user_page user_pages[MAX_PROCESS_PAGES];
	int kernel_pages_count;
	unsigned long kernel_pages[MAX_PROCESS_PAGES];
};

struct task_struct {
	struct tss_struct tss;
	long pid;	
	long counter;
	long priority;
	long preempt_count;
	unsigned short add_priority;
        struct mm_struct mm;
};

extern void sched_init(void);
extern void schedule(void);
extern void do_timer(void);
extern void preempt_disable(void);
extern void preempt_enable(void);
extern void switch_to(struct task_struct* next);
//extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);

#define INIT_TASK \
/*tss_struct*/	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	0,0,5,0,5 \
}

#endif
