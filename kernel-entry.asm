;;kernel-entry.asm - Compile with NASM

bits 32                 ;This kernel is 32-bit

section .text
    ;multiboot spec
    align 4             ;must be 4-byte aligned
    dd 0x1BADB002       ;magic field to identify the multiboot header
    dd 0x00             ;flags (don't care for now)
    dd - (0x1BADB002 + 0x00)    ;checksum. magic + flags + checksum should be 0

global start            ;make our entry-point globally visible
global read_port
global write_port
global load_idt
global keyboard_handler
global move_cursor

extern kmain            ;kmain is the kernel main function in the C file
extern keyboard_handler_main    ;the keryboard handler function in the C file

;The entry point of the kernel.
start:
    cli                     ;clear interrupts (so our kernel isn't woken up after we halt it)
    mov esp, stack_space    ;set up the stack pointer to our stack space
    call kmain              ;call our C kernel's main function
    hlt                     ;halt the cpu (our kernel is done)

;Read from a port
read_port:
    mov edx, [esp + 4]  ;The port number comes in as an argument (on the stack), so move it to edx
    in al, dx           ;Read from port dx and store the byte in al (lower 8 bits of eax)
    ret

;Write to a port
write_port:
    mov edx, [esp + 4]  ;The port number comes in as an argument (on the stack), so we move it to edx
    mov al, [esp + 8]   ;The data to be written comes in as an argument too, so we move it to al
    out dx, al          ;Write the value in al to the port dx.
    ret

;TODO: Finish the following function
;Move the framebuffer cursor to the given row and colum
move_cursor:
    ; start by moving to row 3 col 1 -> 240 + 1 -> 0xF1
    mov al, 14
    mov dx, 0x3D4
    out dx, al   ; 14 signifies HIGH bits are coming
    mov al, 0
    mov dx, 0x3D5
    out dx, al

    mov al, 15
    mov dx, 0x3D4
    out dx, al   ; 15 signifies LOW bits are coming
    mov al, 0xF1
    mov dx, 0x3D5
    out dx, al
    ret

;Load the pointer to the IDT and enable interrupts
load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

;Handle a keyboard interrupt
keyboard_handler:
    call keyboard_handler_main
    iretd

section .bss
resb 8192               ;8KB reserved for our stack
stack_space:

