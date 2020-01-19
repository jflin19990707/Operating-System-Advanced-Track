// AArch64 mode
 
// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.globl _start
 
    .org 0x80000
// Entry point for the kernel. Registers are not defined as in AArch32.
_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // cpu id > 0, stop
    bl      _el2_setup
1:  wfe
    b       1b
2:  // cpu id == 0
 
    // set stack before our code
    ldr     x1, =_start
    mov     sp, x1
 
    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b
    //spsr
_el2_setup:  
    mov     x0, #(0x3c5) //(PSR_F_BIT|PSR_I_BIT|PSR_A_BIT|PSR_D_BIT|PSR_MODE_EL1h)
    msr     spsr_el2, x0
    msr     elr_el2,x30
    mov     w0, #(0xe12) //#BOOT_CPU_MODE_EL2          // This CPU booted in EL2
    eret

 
    // jump to C code, should not return
4:  bl      kernel_main
    // for failsafe, halt this core too
    b 1b
