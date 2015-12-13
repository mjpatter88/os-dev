/*
* link.ld
*/

OUTPUT_FORMAT(elf32-i386)	/* Set our output format to be 32-bit ELF */
ENTRY(start)               /* Set the "start" symbol name to be the entry point of the executable. */
SECTIONS
{
    . = 0x100000;           /* Set the location counter (".") to be 0x100000 */
    .text : { *(.text) }    /* Merge all .text sections and place them at 0x100000 */
    .data : { *(.data) }
    .bss : { *(.bss) }
}
