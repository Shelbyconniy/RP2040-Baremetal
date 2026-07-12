#include <stdint.h>
#include "interrupts.h"
#include "mmio.h"
#include "time.h"
#include "gpio.h"

void sleep_us(uint32_t delay, uint32_t alarm) {
    uint32_t start_time = mmio_read_32(TIME_L_READ);
    uint32_t target_time = start_time + delay;
    mmio_write_32(TIMER_ALARM0 + (ALARM_SIZE * alarm), target_time);

    setBits(TIME_INTE, (1 << alarm));

    if (mmio_read_32(TIME_L_READ) - start_time >= delay) {
        clearBits(TIME_INTE, (1 << 0));
        return;
    }

    while(1) {
        __disable_irq();
        if (mmio_read_32(TIME_L_READ) - start_time >= delay) {
            clearBits(TIME_INTE, (1 << alarm));
            __enable_irq();
            break;
        }
        __WFI();
        __enable_irq();
    }
}

void Timer0_IRQHandler() {
    mmio_write_32(TIME_INTR, (1 << 0));
    NVIC_ClearPendingIRQ(TIMER_IRQ_0);
}

void Timer1_IRQHandler() {
    mmio_write_32(TIME_INTR, (1 << 1));
    NVIC_ClearPendingIRQ(TIMER_IRQ_1);
}
