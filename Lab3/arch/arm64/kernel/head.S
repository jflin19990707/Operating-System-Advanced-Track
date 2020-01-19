// AArch64 mode 
// To keep this in the first portion of the binary.
.section ".text.boot"

// Make _start global.
.globl _start
_start:
    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:
    // jump to C code, should not return
    b      _el2_setup
    // set stack before our code
flag:
    ldr     x1, =_start
    mov     sp, x1

4:  b       kernel_main
    //spsr
_el2_setup:
    mov     x0, #(0x3c5) //(PSR_F_BIT|PSR_I_BIT|PSR_A_BIT|PSR_D_BIT|PSR_MODE_EL1h)
    msr     spsr_el2, x0
    adr     x0, flag
    msr     elr_el2,x0
    mov     x0, #(0x0000EFFA)
    msr     sctlr_el1, x0
    mov     x0, #(1<<31)
    msr     hcr_el2, x0
    eret

 

