#include <stdint.h>
#pragma once

static inline void mmio_write_32(uint32_t addr, uint32_t value) {
    *(volatile uint32_t *)addr = value;
}
static inline uint32_t mmio_read_32(uint32_t addr) {
    return *(volatile uint32_t *)addr;
}

/* Peripheral atomic access */

#define XOR_OFFSET 0x1000
#define SET_OFFSET 0x2000
#define CLR_OFFSET 0x3000

static inline void setBits(uint32_t addr, uint32_t value) {
    mmio_write_32(addr + SET_OFFSET, value);
}

static inline void clearBits(uint32_t addr, uint32_t value) {
    mmio_write_32(addr + CLR_OFFSET, value);
}

static inline void invertBits(uint32_t addr, uint32_t value) {
    mmio_write_32(addr + XOR_OFFSET, value);
}

/* Cortex-M barriers */

static inline void __DMB() {
    asm volatile ("dmb" : : : "memory");
}

static inline void __DSB() {
    asm volatile ("dsb" : : : "memory");
}

static inline void __ISB() {
    asm volatile ("isb" : : : "memory");
}
