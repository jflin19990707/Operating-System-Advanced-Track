#include "mm.h"
#include "mmu.h"

static unsigned short mem_map[PAGING_PAGES] = {
    0,
};

unsigned long get_free_page()
{
    for (int i = 0; i < PAGING_PAGES; i++)
    {
        if (mem_map[i] == 0)
        {
            mem_map[i] = 1;
            return LOW_MEMORY + i * PAGE_SIZE;
        }
    }
    return 0;
}

void map_page(unsigned long *table, unsigned long shift, unsigned long flag, unsigned long va, unsigned long pa)
{   
    unsigned long index = va >> shift;
    index = index & (PTRS_PER_TABLE - 1);
    unsigned long entry = pa | flag;
    table[index] = entry;
}

void map_table(unsigned long *table, unsigned long shift, unsigned long va, unsigned long *next_level_table)
{
    unsigned long index = va >> shift;
    index = index & (PTRS_PER_TABLE - 1);
    unsigned long entry = (unsigned long)next_level_table | MM_TYPE_PAGE_TABLE;
    table[index] = entry;
}

void map_block_mem(unsigned long *table, unsigned long shift, unsigned long flag, unsigned long va_start, unsigned long va_end, unsigned long pa)
{
    unsigned long index = va_start >> shift;
    index = index & (PTRS_PER_TABLE - 1);
    unsigned long end_index = va_end >> shift;
    end_index = end_index & (PTRS_PER_TABLE - 1);
    while (index <= end_index)
    {
        table[index] = pa | flag;
        pa += PAGE_SIZE;
        index += 1;
    }
}

void create_mapping()
{
    extern unsigned long pg_dir;
    extern unsigned long text_begin;
    extern unsigned long text_end;
    extern unsigned long rodata_begin;
    extern unsigned long rodata_end;
    extern unsigned long data_begin;
    //extern unsigned long stack_top;
    extern unsigned long kernel_end;
    unsigned long pg_dir_asm;
    __asm__(
        "mrs %0,ttbr0_el1"
        :"=r"(pg_dir_asm)
        );

    unsigned long *pmd_addr = (unsigned long *)(pg_dir_asm + 2 * PAGE_SIZE);
    unsigned long *pte_addr = (unsigned long *)(pg_dir_asm + 3 * PAGE_SIZE);

    map_block_mem(pte_addr, PAGE_SHIFT, MMU_FLAGS | MM_READONLY_FLAG,(unsigned long)&text_begin, (unsigned long)&text_end, ((unsigned long)(char*)&text_begin+0x80000));
    map_block_mem(pte_addr, PAGE_SHIFT, MMU_FLAGS | MM_READONLY_FLAG | MM_NOT_EXECUTABLE_FLAG,
                  (unsigned long)&rodata_begin, (unsigned long)&rodata_end, ((unsigned long)(char*)&rodata_begin+0x80000));
    map_block_mem(pte_addr, PAGE_SHIFT, MMU_FLAGS,
                  (unsigned long)&data_begin, (unsigned long)&kernel_end - PAGE_SIZE, ((unsigned long)&data_begin+0x80000));
    map_block_mem(pte_addr, PAGE_SHIFT, MMU_FLAGS | MM_NOT_EXECUTABLE_FLAG, 0x100000, 0x200000 - PAGE_SIZE, 0x100000);

    int i;

    for (i = 1; i < 8; i++)
    {
        pte_addr = (unsigned long *)((char *)pte_addr + PAGE_SIZE);
        map_table(pmd_addr, PMD_SHIFT, i * SECTION_SIZE, pte_addr);
        map_block_mem(pte_addr, PAGE_SHIFT, MMU_FLAGS | MM_NOT_EXECUTABLE_FLAG, i * SECTION_SIZE, (i + 1) * SECTION_SIZE - PAGE_SIZE, i * SECTION_SIZE);
    }

    pte_addr = (unsigned long *)((char *)pte_addr + PAGE_SIZE);
    map_table(pmd_addr, PMD_SHIFT, 0x1000000, pte_addr);
    map_page(pte_addr, PAGE_SHIFT, MMU_FLAGS, V_UART0_ADDR, UART0_ADDR_PAGE);
    map_page(pte_addr, PAGE_SHIFT, MMU_FLAGS, V_LOCAL_TIMER_PAGE, LOCAL_TIMER_PAGE);

}
