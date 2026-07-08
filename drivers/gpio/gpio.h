#include "sio.h"
#include "mmio.h"

#define GPIO(X)             (1UL << X)

/* I/O value registers */
#define GPIO_INPUT_VAL      (SIO_BASE + 0x004) // [29:0] RO
#define GPIO_OUTPUT_VAL     (SIO_BASE + 0x010) // [29:0] RW

/* Output GPIO write-only registers */
#define GPIO_OUTPUT_SET     (SIO_BASE + 0x014) // [29:0] WO
#define GPIO_OUTPUT_CLR     (SIO_BASE + 0x018) // [29:0] WO
#define GPIO_OUTPUT_XOR     (SIO_BASE + 0x01C) // [29:0] WO

/* I/0 registers */
/* 1: Output, 0: Input */
#define GPIO_OUTPUT_EN      (SIO_BASE + 0x020) // [29:0] RW
#define GPIO_OUTPUT_EN_SET  (SIO_BASE + 0x024) // [29:0] WO
#define GPIO_OUTPUT_EN_CLR  (SIO_BASE + 0x028) // [29:0] WO
#define GPIO_OUTPUT_EN_XOR  (SIO_BASE + 0x02C) // [29:0] WO

/* Function select registers */
#define GPIO_BASE           0x40014000
#define GPIO_STATUS_BASE    (GPIO_BASE + 0x000)
#define GPIO_CTRL_BASE      (GPIO_BASE + 0x004)
/*
[31:30] Reserved
[29:28] IRQOVER
    - 0x0 -> Normal
    - 0x1 -> Invert the interrupt
    - 0x2 -> Drive the interrupt low
    - 0x3 -> Drive the interrupt high
[27:18] Reserved
[17:16] INOVER
    - 0x0 -> Normal
    - 0x1 -> Invert the input signal 
    - 0x2 -> Drive the input signal low
    - 0x3 -> Drive the input signal high
[15:14] Reserved
[13:12] OEOVER
    - 0x0 -> Normal
    - 0x1 -> Invert output enable value
    - 0x2 -> Disable output (forced)
    - 0x3 -> Enable output (forced)
[11:10] Reserved
[9:8]   OUTOVER
    - 0x0 -> Normal
    - 0x1 -> Invert the output signal 
    - 0x2 -> Drive the output signal low
    - 0x3 -> Drive the output signal high
[7:6]   Reserved
[4:0]   FUNCSEL
*/
#define GPIO_SIZE           0x08

#define GPIO_INTR_BASE      (GPIO_BASE + 0x0F0)

#define GPIO_INTE_BASE      (GPIO_BASE + 0x100)
#define GPIO_INTF_BASE      (GPIO_BASE + 0x110)
#define GPIO_INTS_BASE      (GPIO_BASE + 0x120)
#define GPIO_CORE_SIZE      0x30

#define GPIO_EDGE_HIGH      0x3
#define GPIO_EDGE_LOW       0x2
#define GPIO_LEVEL_HIGH     0x1
#define GPIO_LEVEL_LOW      0x0

static inline void gpio_set(uint32_t gpio_num) {
    mmio_write_32(GPIO_OUTPUT_SET, GPIO(gpio_num));
}

static inline void gpio_clear(uint32_t gpio_num) {
    mmio_write_32(GPIO_OUTPUT_CLR, GPIO(gpio_num));
}

static inline void gpio_invert(uint32_t gpio_num) {
    mmio_write_32(GPIO_OUTPUT_XOR, GPIO(gpio_num));
}

static inline uint32_t gpio_read(uint32_t gpio_num) {
    return (mmio_read_32(GPIO_INPUT_VAL) & GPIO(gpio_num));
}

static inline void gpio_mode_output(uint32_t gpio_num) {
    mmio_write_32(GPIO_OUTPUT_EN_SET, GPIO(gpio_num));
}

static inline void gpio_mode_input(uint32_t gpio_num) {
    mmio_write_32(GPIO_OUTPUT_EN_CLR, GPIO(gpio_num));
}


static inline void gpio_enable_interrupt(uint32_t core, uint32_t gpio_num, uint32_t int_mode) {
    uint32_t addr = GPIO_INTE_BASE + (GPIO_CORE_SIZE * core) + (0x04 * (gpio_num / 8));
    uint32_t mask = 1 << (((gpio_num % 8) * 4) + int_mode);
    setBits(addr, mask);
}

static inline void gpio_mask_interrupt(uint32_t core, uint32_t gpio_num, uint32_t int_mode) {
    uint32_t addr = GPIO_INTE_BASE + (GPIO_CORE_SIZE * core) + (0x04 * (gpio_num / 8));
    uint32_t mask = 1 << (((gpio_num % 8) * 4) + int_mode);
    clearBits(addr, mask);
}

static inline void pinmux_set(uint32_t gpio_num, uint32_t func) {
    clearBits(GPIO_CTRL_BASE + GPIO_SIZE * gpio_num, 0x1F);
    setBits(GPIO_CTRL_BASE + GPIO_SIZE * gpio_num, func);
}
