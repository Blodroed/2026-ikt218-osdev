#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <libc/stdint.h>

#define IDT_ENTRIES 256

struct IdtEntry{
	uint16_t    interrupt_low;
	uint16_t    kernel_cs;
	uint8_t     reserved;
	uint8_t     attributes;
	uint16_t    interrupt_high;
} __attribute__((packed));

struct Idtr {
    uint16_t    limit;
    uint32_t    base;
} __attribute__((packed));

void IdtInitialize(void);
void IdtSetDescriptor(uint8_t vector, uint32_t interrupt, uint8_t flags);

#endif