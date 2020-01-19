#ifndef _MM_H
#define _MM_H

#include "base.h"

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY 0x65000 //SECTION_SIZE
#define HIGH_MEMORY 0x80000		//SECTION_SIZE * 2

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES (PAGING_MEMORY / PAGE_SIZE)

#define PTRS_PER_TABLE (1 << TABLE_SHIFT)

#define VA_START 0
#define PA_START 0x80000
#define VA_END 0x80000
#define PA_END 0x100000

#define PGD_SHIFT PAGE_SHIFT + 3 * TABLE_SHIFT
#define PUD_SHIFT PAGE_SHIFT + 2 * TABLE_SHIFT
#define PMD_SHIFT PAGE_SHIFT + TABLE_SHIFT
#define PG_DIR_SIZE (3 * PAGE_SIZE)

#define UART0_ADDR 0x3f201000
#define UART0_ADDR_PAGE 0x3f201000
#define V_UART0_ADDR 0x1001000
#define LOCAL_TIMER_PAGE 0x40000000
#define V_LOCAL_TIMER_PAGE 0x1000000
#define V_LOCAL_TIMER_CONTROL 0x1000034
#define V_LOCAL_TIMER_CLEAR 0x1000038
#define V_LOCAL_INTERRUPT_ROUTE 0x1000024
#define V_INTERRUPT_SOURCE_0 0x1000060

#ifndef __ASSEMBLER__

unsigned long get_free_page();
void map_page(unsigned long *table, unsigned long shift, unsigned long flag, unsigned long va, unsigned long pa);
void create_mapping();
// void free_page(unsigned long p);
// void memzero(unsigned long src, unsigned long n);

#endif

#endif /*_MM_H */
