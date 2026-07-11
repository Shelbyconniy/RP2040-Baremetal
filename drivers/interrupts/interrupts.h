#include <stdint.h>
#include "sys_reg.h"
#include "mmio.h"

#define NVIC_IRQ_SET_EN         (CORTEX_BASE + 0xE100) // RW
#define NVIC_IRQ_CLR_EN         (CORTEX_BASE + 0xE180) // RW
#define NVIC_IRQ_SET_PENDING    (CORTEX_BASE + 0xE200) // RW
#define NVIC_IRQ_CLR_PENDING    (CORTEX_BASE + 0xE280) // RW

/* Interrupt priority registers (from 0 to 3) */

#define NVIC_IRQ_PR0        (CORTEX_BASE + 0xE400) // RW
/* [7:6]   Interrupt 0 */
/* [15:14] Interrupt 1 */
/* [23:22] Interrupt 2 */
/* [31:30] Interrupt 3 */

#define NVIC_IRQ_PR1        (CORTEX_BASE + 0xE404) // RW
/* [7:6]   Interrupt 4 */
/* [15:14] Interrupt 5 */
/* [23:22] Interrupt 6 */
/* [31:30] Interrupt 7 */

#define NVIC_IRQ_PR2        (CORTEX_BASE + 0xE408) // RW
/* [7:6]   Interrupt 8 */
/* [15:14] Interrupt 9 */
/* [23:22] Interrupt 10 */
/* [31:30] Interrupt 11 */

#define NVIC_IRQ_PR3        (CORTEX_BASE + 0xE40C) // RW
/* [7:6]   Interrupt 12 */
/* [15:14] Interrupt 13 */
/* [23:22] Interrupt 14 */
/* [31:30] Interrupt 15 */

#define NVIC_IRQ_PR4        (CORTEX_BASE + 0xE410) // RW
/* [7:6]   Interrupt 16 */
/* [15:14] Interrupt 17 */
/* [23:22] Interrupt 18 */
/* [31:30] Interrupt 19 */

#define NVIC_IRQ_PR5        (CORTEX_BASE + 0xE414) // RW
/* [7:6]   Interrupt 20 */
/* [15:14] Interrupt 21 */
/* [23:22] Interrupt 22 */
/* [31:30] Interrupt 23 */

#define NVIC_IRQ_PR6        (CORTEX_BASE + 0xE418) // RW
/* [7:6]   Interrupt 24 */
/* [15:14] Interrupt 25 */
/* [23:22] Interrupt 26 */
/* [31:30] Interrupt 27 */

#define NVIC_IRQ_PR7        (CORTEX_BASE + 0xE41C) // RW
// /* [7:6]   Interrupt 28 */
// /* [15:14] Interrupt 29 */
// /* [23:22] Interrupt 30 */
// /* [31:30] Interrupt 31 */

/* Interrupt vector table pointer */
#define VTOR_ADDR           (CORTEX_BASE + 0xED08)

#define IRQ_OFFSET          16

/* IRQ definitions */
#define TIMER_IRQ_0         0
#define TIMER_IRQ_1         1
#define TIMER_IRQ_2         2
#define TIMER_IRQ_3         3
#define PWM_IRQ             4
#define USB_IRQ             5
#define XIP_IRQ             6
#define PIO0_IRQ_0          7
#define PIO0_IRQ_1          8
#define PIO1_IRQ_0          9
#define PIO1_IRQ_1          10
#define DMA_IRQ_0           11
#define DMA_IRQ_1           12
#define GPIO_BANK0_IRQ      13
#define GPIO_QSPI_IRQ       14 
#define FIFO_CORE0_IRQ      15
#define FIFO_CORE1_IRQ      16
#define CLOCKS_IRQ          17
#define SPI0_IRQ            18
#define SPI1_IRQ            19
#define UART0_IRQ           20
#define UART1_IRQ           21
#define ADC_FIFO_IRQ        22
#define I2C0_IRQ            23
#define I2C1_IRQ            24
#define RTC_IRQ             25


static inline void NVIC_EnableIRQ(uint32_t irq_number) {
    mmio_write_32(NVIC_IRQ_SET_EN, (1 << irq_number));
    __DSB();
    __ISB();
}

static inline void NVIC_DisableIRQ(uint32_t irq_number) {
    mmio_write_32(NVIC_IRQ_CLR_EN, (1 << irq_number));
    __DSB();
    __ISB();
}

static inline void NVIC_SetPendingIRQ(uint32_t irq_number) {
    mmio_write_32(NVIC_IRQ_SET_PENDING, (1 << irq_number));
    __DSB();
    __ISB();
}

static inline void NVIC_ClearPendingIRQ(uint32_t irq_number) {
    mmio_write_32(NVIC_IRQ_CLR_PENDING, (1 << irq_number));
    __DSB();
    __ISB();
}
