// AArch64 mode 
// To keep this in the first portion of the binary.
#include "mm.h"
#include "mmu.h"
#include "sysregs.h"
.section ".text.boot"

// Make _start global.
.globl _start
_start:
    bl disable_irq
    bl _el2_setup

el1_entry:
    adr     x0, __bss_start
    adr     x1, __bss_size
    
    bl memzero
    bl __create_page_tables

    ldr     x0, =stack_top
    mov     sp, x0

// 需要打开SCTLR_MMU后，再进入kernel_main
open_mmu:    
    //mov x0, #VA_START           
    //add sp, x0, #LOW_MEMORY
    adrp x0, pg_dir
    msr ttbr0_el1, x0

    ldr x0, =(TCR_VALUE)
    msr tcr_el1, x0

    ldr x0, =(MAIR_VALUE)
    msr mair_el1, x0

    ldr x2, =kernel_main

    mov x0, #SCTLR_MMU_ENABLED
    msr sctlr_el1, x0

    br  x2

    .macro create_pgd_entry, tbl, virt, tmp1, tmp2
    create_table_entry \tbl, \virt, PGD_SHIFT, \tmp1, \tmp2
    create_table_entry \tbl, \virt, PUD_SHIFT, \tmp1, \tmp2
	create_table_entry \tbl, \virt, PMD_SHIFT, \tmp1, \tmp2
    .endm

    .macro create_table_entry, tbl, virt, shift, tmp1, tmp2
    lsr \tmp1, \virt, #\shift
    and \tmp1, \tmp1, #PTRS_PER_TABLE - 1   // table index
    add \tmp2, \tbl, #PAGE_SIZE
    orr \tmp2, \tmp2, #MM_TYPE_PAGE_TABLE
    str \tmp2, [\tbl, \tmp1, lsl #3]
    add \tbl, \tbl, #PAGE_SIZE     // next level table page
    .endm

    .macro create_block_map, tbl, phys, start, end, flags, tmp1
    lsr \start, \start, #PAGE_SHIFT
    and \start, \start, #PTRS_PER_TABLE - 1   // table index
    lsr \end, \end, #PAGE_SHIFT
    and \end, \end, #PTRS_PER_TABLE - 1    // table end index
    lsr \phys, \phys, #PAGE_SHIFT
    mov \tmp1, #\flags
    orr \phys, \tmp1, \phys, lsl #PAGE_SHIFT   // table entry
9999: str \phys, [\tbl, \start, lsl #3]    // store the entry
    add \start, \start, #1      // next entry
    add \phys, \phys, #PAGE_SIZE    // next page
    cmp \start, \end
    b.ls 9999b
    .endm

__create_page_tables:
    mov x29, x30      // save return address

    adrp x0, pg_dir
    mov x1, #VA_START
    create_pgd_entry x0, x1, x2, x3
    
    // vmlinux.lds.S stores Virtual Address
    // map virt(0-512k) to phys(512k-1mb)
    mov  x1, #PA_START
    mov  x2, #VA_START
    ldr x3, =VA_END  // last virtual address
    create_block_map x0, x1, x2, x3, MMU_FLAGS, x4

    // map virt(512k-1mb) to phys(512k-1mb)
    mov x1, #PA_START
    mov x2, #PA_START
    ldr x3, =PA_END  // last virtual address
    create_block_map x0, x1, x2, x3, MMU_FLAGS, x4


    mov x30, x29      // restore return address
    ret

_el2_setup:
	mrs	x0, CurrentEL
	cmp	x0, #(2 << 2)
	b.ne	1f
	mrs	x0, sctlr_el2
	msr	sctlr_el2, x0

1:	/* spsr */
	mov	x0, #(0x3c5)
	msr	spsr_el2, x0
	msr	elr_el2, x30
	mov	w20, #(0xe12)     // This CPU booted in EL2
	eret

memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	bgt memzero
	ret

