#include <libc/stdint.h>
#include "gdt.h"

void main(void)
{
    gdt_init();

    volatile uint16_t *video = (volatile uint16_t *)0xB8000;
    const char *text = "Hello World";

    for (int i = 0; text[i] != '\0'; i++)
    {
        video[i] = (uint16_t)text[i] | (uint16_t)0x0F00;
    }

    while (1)
    {
        __asm__ volatile("hlt");
    }
}