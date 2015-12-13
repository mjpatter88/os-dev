/*
*   kernel.c 
*/

void kmain(void)
{
    const char *str = "Hello, World";
    char *vidptr = (char*)0xb8000;          // A pointer to the start of video memory

    // Clear the screen. 25 lines, each of 80 columns, each position having 2 bytes
    for(int i=0; i<(80*25*2); i=i+2) {
        vidptr[i] = ' ';    // blank character
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
    }

    // Write the string to the screen
    unsigned int i = 0;
    unsigned int j = 0;
    while(str[j] != '\0') {
        vidptr[i] = str[j];
        vidptr[i+1] = 0x07; // attribute-byte, light-grey on black screen
        j++;
        i+=2;
    }
}
