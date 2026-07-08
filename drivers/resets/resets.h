#include <stdint.h>
#include "mmio.h"

#define RESET           0x4000C000 // RW
/*
[31:25] Reserved
[24]    USB
[23]    UART1
[22]    UART0
[21]    TIMER
[20]    TBMAN
[19]    SYSINFO
[18]    SYSCONFIG
[17]    SPI1
[16]    SPI0
[15]    RTC
[14]    PWM
[13]    PLL_USB
[12]    PLL_SYS
[11]    PIO1
[10]    PIO0
[9]     PADS_QSPI
[8]     PADS_BANK0
[7]     JTAG
[6]     IO_ASPI
[5]     IO_BANK0
[4]     I2C1
[3]     I2C0
[2]     DMA
[1]     BUSCTRL
[0]     ADC
*/

#define UART0_RST       (1 << 22) 
#define TIMER_RST       (1 << 21)
#define PLL_SYS_RST     (1 << 12)
#define PIO1_RST        (1 << 11)
#define PIO0_RST        (1 << 10)
#define GPIO_RST        (1 << 5)
#define DMA_RST         (1 << 2)

#define RESET_DONE      (RESET + 0x008) // RO

static inline void enable_devices (uint32_t resets) {
    clearBits(RESET, resets);
    while((mmio_read_32(RESET_DONE) & (resets)) != resets);
}