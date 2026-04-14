#include <kernel/interrupt.h>
#include <kernel/terminal.h>
#include <kernel/io.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20

static InterruptHandler interruptHandlers[256];

const char* isrMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void PicSendEoi(uint8_t irqNum) {
    if (irqNum >= 8) {
        OutPortByte(PIC2_COMMAND, PIC_EOI);
    }
    OutPortByte(PIC1_COMMAND, PIC_EOI);
}

void PicRemap(void) {
    OutPortByte(PIC1_COMMAND, 0x11);
    OutPortByte(PIC2_COMMAND, 0x11);

    OutPortByte(PIC1_DATA, 0x20);
    OutPortByte(PIC2_DATA, 0x28);

    OutPortByte(PIC1_DATA, 0x04);
    OutPortByte(PIC2_DATA, 0x02);

    OutPortByte(PIC1_DATA, 0x01);
    OutPortByte(PIC2_DATA, 0x01);

    OutPortByte(PIC1_DATA, 0x00);
    OutPortByte(PIC2_DATA, 0x00);
}

void IsrHandler(struct Registers* registers) {
    TerminalWriteString("\n=== INTERRUPT RECEIVED ===\n");

    TerminalWriteString("Interrupt Number: ");
    TerminalWriteUInt(registers->int_no);
    TerminalWriteString("\n");

    TerminalWriteString("Message: ");
    if (registers->int_no < 32) {
        TerminalWriteString(isrMessages[registers->int_no]);
    } else {
        TerminalWriteString("Unknown Interrupt");
    }
    TerminalWriteString("\n");

    TerminalWriteString("Error Code: ");
    TerminalWriteUInt(registers->err_code);
    TerminalWriteString("\n");

    TerminalWriteString("==========================\n");

    for (;;) {
        __asm__ volatile("cli; hlt");
    }
}

void RegisterInterruptHandler(uint8_t iNum, InterruptHandler handler) {
    interruptHandlers[iNum] = handler;
}

void IrqHandler(struct Registers* registers) {
    uint8_t irqNum = (uint8_t)(registers->int_no - 32);

    if (interruptHandlers[registers->int_no] != 0) {
        interruptHandlers[registers->int_no](registers);
    } else if (irqNum != 0) {
        TerminalWriteString("IRQ triggered: ");
        TerminalWriteUInt(irqNum);
        TerminalWriteString("\n");
    }

    PicSendEoi(irqNum);
}
