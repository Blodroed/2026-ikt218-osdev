#include <libc/stdint.h>
#include <kernel/terminal.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/interrupt.h>
#include <kernel/keyboard.h>

void main(void) {
    TerminalInitialize();
    GdtInitialize();
    IdtInitialize();

    RegisterInterruptHandler(IRQ1, KeyboardHandler);

    for (;;) {
        __asm__ volatile("hlt");
    }
}