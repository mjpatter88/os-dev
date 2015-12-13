;;kernel-entry.asm - Compile with NASM

bits 32                 ;This kernel is 32-bit

section .text
    ;multiboot spec
    align 4             ;must be 4-byte aligned
    dd 0x1BADB002       ;magic field to identify the multiboot header
    dd 0x00             ;flags (don't care for now)
    dd - (0x1BADB002 + 0x00)    ;checksum. magic + flags + checksum should be 0

global start            ;make our entry-point globally visible
extern kmain            ;kmain is the kernel main function in the C file

start:
    cli                     ;clear interrupts (so our kernel isn't woken up after we halt it)
    mov esp, stack_space    ;set up the stack pointer to our stack space
    call kmain              ;call our C kernel's main function
    hlt                     ;halt the cpu (our kernel is done)

section .bss
resb 8192               ;8KB reserved for our stack
stack_space:

