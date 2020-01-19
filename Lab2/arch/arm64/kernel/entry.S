
.macro ventry label
	.align 7
	b \label
	.endm

.globl vectors
.align 11
vectors:
	ventry	el1_sync_invalid		// Synchronous EL1t
	ventry	el1_irq_invalid			// IRQ EL1t
	ventry	el1_fiq_invalid			// FIQ EL1t
	ventry	el1_error_invalid		// Error EL1t

	ventry	el1_sync			// Synchronous EL1h
	ventry	el1_irq				// IRQ EL1h
	ventry	el1_fiq_invalid			// FIQ EL1h
	ventry	el1_error_invalid		// Error EL1h

	ventry	el0_sync			// Synchronous 64-bit EL0
	ventry	el0_irq				// IRQ 64-bit EL0
	ventry	el0_fiq_invalid			// FIQ 64-bit EL0
	ventry	el0_error_invalid		// Error 64-bit EL0

	ventry	el0_sync_invalid		// Synchronous 32-bit EL0
	ventry	el0_irq_invalid			// IRQ 32-bit EL0
	ventry	el0_fiq_invalid			// FIQ 64-bit EL0
	ventry	el0_error_invalid		// Error 64-bit EL0
	
.macro	kernel_entry, regsize = 64
	stp	x0, x1, [sp, #16 * 0]
	stp	x2, x3, [sp, #16 * 1]
	stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]
	mrs	x21, elr_el1
	mrs	x22, spsr_el1
	.endm

	.macro	kernel_exit
	msr	elr_el1, x21			// set up the return data
	msr	spsr_el1, x22
	ldp	x0, x1, [sp, #16 * 0]
	ldp	x2, x3, [sp, #16 * 1]
	ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]
	eret	
	.endm

.globl init_irq
init_irq:
	ldr	x5, =vectors
	msr     vbar_el1, x5
	msr	daifclr, #2
	ret


el1_sync_invalid:
	kernel_entry
	kernel_exit

el1_irq_invalid:
	kernel_entry
	kernel_exit

el1_fiq_invalid:
	kernel_entry
	kernel_exit

el1_error_invalid:
	kernel_entry
	kernel_exit

el1_sync:
	kernel_entry
	kernel_exit

el1_irq:
	kernel_entry
	bl irq_main
	kernel_exit

el0_sync:
	kernel_entry
	kernel_exit

el0_irq:
	kernel_entry
	kernel_exit

el0_fiq_invalid:
	kernel_entry
	kernel_exit

el0_error_invalid:
	kernel_entry
	kernel_exit

el0_sync_invalid:
	kernel_entry
	kernel_exit

el0_irq_invalid:
	kernel_entry
	kernel_exit
