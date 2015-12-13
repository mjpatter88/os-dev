# os-dev
operating system development work

### Commands
"qemu-system-i386 -kernel kernel" will launch qemu running the kernel.
"make" should build everything (nasm, gcc, and ld commands)
"make clean" should clean out all object and executable files.


### How an x86 computer boots
- The CPU is hardwired to begin execution at address 0xFFFFFFF0.
- This address contains a jump instruction to the BIOS.
- The BIOS searches for a bootable device by looking for a "magic number" that all bootable devices must have.
- The BIOS copies the device's first sector into RAM at 0x7c00 and then jumps/executes that code.
- This piece of code is called a bootloader, and it usually just loads the kernel at address 0x100000 and jumps to it.
- In order to write a kernel, we just need to make sure the linker loads our binary at address 0x100000.


### A Note on C and Assembly
Assembly cannot be completely avoided. At the very least, we need a small file that serves as the entry point into our kernel
and then jumps to an externel function that we will write in C.


### Adresses
- 0xb8000 - the start of video memory in protected mode.


### Links
- http://arjunsreedharan.org/post/82710718100/kernel-101-lets-write-a-kernel
