#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <libc/stdint.h>

static inline void OutPortByte(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t InPortByte(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void IoWait(void) {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

#endif