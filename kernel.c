/*
*   kernel.c 
*/
#include "keyboard_map.h"

#define IDT_SIZE 256
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

struct IDT_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern void move_cursor(void);

unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;          // A pointer to the start of video memory

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


    // ICW1 (initialization)
    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);

    // ICW2 (set offsets)
    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);

    // ICW3 (disable cascading)
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);

    // ICW4 (set environment to x86)
    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    // Turn off all interrupts
    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    // Populate the IDT descriptor

    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
}

void kb_init(void)
{
    // 0xFD is 11111101 - enable only IRQ1 (keyboard)
    write_port(0x21, 0xFD);
}

void keyboard_handler_main(void)
{
    unsigned char status;
    char keycode;

    // Signal EOI
    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);
    if(status & 0x1)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if(keycode < 0)
        {
            return;
        }
        char cur_char = keyboard_map[keycode];
        if(cur_char ==  '\b')
        {
            current_loc -= 2;
            vidptr[current_loc] = ' ';
        }
        else
        {
            vidptr[current_loc++] = keyboard_map[keycode];
            vidptr[current_loc++] = 0x07;
        }
    }
}

void kmain(void)
{
    const char *str = "Hello, World!";
    const char *str2 = "Try typing a few characters at the following prompt.";

    // Clear the screen. 25 lines, each of 80 columns, each position having 2 bytes
    unsigned int i = 0;
    for(i; i<(80*25*2); i=i+2) {
        vidptr[i] = ' ';    // blank character
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
    }

    // Write the first string to the screen
    i = 0;
    unsigned int j = 0;
    while(str[j] != '\0') {
        vidptr[i] = str[j];
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
        j++;
        i+=2;
    }

    // Write the second string to the screen
    int row = 1;
    i = 80*row*2;
    j = 0;
    while(str2[j] != '\0') {
        vidptr[i] = str2[j];
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
        j++;
        i+=2;
    }

    // Write the console prompt
    row = 3;
    i = 80*row*2;
    vidptr[i] = '>';
    vidptr[i+1] = 0x07;

    // Move the cursor (TODO:start here, move to appropriate place)
    move_cursor();

    idt_init();
    kb_init();

    while(1);
}
