#pragma once

#include <stdint.h>

#define CORTEX_BASE 0xE0000000


static inline void __WFI() {
    asm volatile ("wfi");
}

static inline void __WFE() {
    asm volatile ("wfe");
}

static inline void __SEV() {
    asm volatile ("sev");
}

static inline void __enable_irq() {
    asm volatile ("cpsie i" : : : "memory");
}

static inline void __disable_irq() {
    asm volatile ("cpsid i" : : : "memory");
}

static inline uint32_t __save_PRIMASK() {
    uint32_t state_var;
    asm volatile("mrs %0, PRIMASK" : "=r" (state_var) :: "memory");
    return state_var;
}

static inline void __set_PRIMASK(uint32_t state_var) {
    asm volatile("msr PRIMASK,%0" :: "r" (state_var) : "memory");
}