/*
*   kernel.c 
*/

struct IDT_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
}

struct IDTentry IDT[IDT_SIZE];

void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    // TODO: Start here and continue implementing the interrupt handler for the keyboard.
}

void kmain(void)
{
    const char *str = "Hello, World";
    char *vidptr = (char*)0xb8000;          // A pointer to the start of video memory

    // Clear the screen. 25 lines, each of 80 columns, each position having 2 bytes
    unsigned int i = 0;
    for(i; i<(80*25*2); i=i+2) {
        vidptr[i] = ' ';    // blank character
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
    }

    // Write the string to the screen
    i = 0;
    unsigned int j = 0;
    while(str[j] != '\0') {
        vidptr[i] = str[j];
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
        j++;
        i+=2;
    }
}
