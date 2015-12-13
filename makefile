all: kernel

kernel: kernel.o kernel-entry.o link.ld
	ld -m elf_i386 -T link.ld -o kernel kernel-entry.o kernel.o

kernel.o: kernel.c
	gcc -m32 -c kernel.c -o kernel.o

kernel-entry.o: kernel-entry.asm
	nasm -f elf32 kernel-entry.asm -o kernel-entry.o


clean:
	rm -f *.o
	rm -f kernel
