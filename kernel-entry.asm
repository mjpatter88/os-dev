;;kernel-entry.asm - Compile with NASM

bits 32                 ;This kernel is 32-bit

section .text

global start            ;make our entry-point globally visible
extern kmain            ;kmain is the kernel main function in the C file

start:
    cli                     ;clear interrupts (so our kernel isn't woken up after we halt it)
    mov esp, stack_space    ;set up the stack pointer to our stack space
    call kmain              ;call our C kernel's main function
    hlt                     ;halt the cpu (our kernel is done)

section .bas
resb 8192               ;8KB reserved for our stack
stack_space:

