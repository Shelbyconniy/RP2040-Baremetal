#include <stdint.h>

#define WATCHDOG_BASE   0x40058000
#define WATCHDOG_TICK   (WATCHDOG_BASE + 0x2C)
//      [9]     Enable (0x1 by default)
//      [8:0]   Cycles (12 for 12 MHz)

#define TIMER_BASE      0x40054000
#define TIME_H_WRITE    (TIMER_BASE + 0x00)
#define TIME_L_WRITE    (TIMER_BASE + 0x04)
#define TIME_H_READ     (TIMER_BASE + 0x08)
#define TIME_L_READ     (TIMER_BASE + 0x0C)

#define TIMER_ALARM0    (TIMER_BASE + 0x10)
#define TIMER_ALARM1    (TIMER_BASE + 0x14)
#define TIMER_ALARM2    (TIMER_BASE + 0x18)
#define TIMER_ALARM3    (TIMER_BASE + 0x1C)

#define TIMER_ARMED     (TIMER_BASE + 0x20)

#define TIME_RAW_H      (TIMER_BASE + 0x24)
#define TIME_RAW_L      (TIMER_BASE + 0x28)

#define TIMER_PAUSE     (TIMER_BASE + 0x30)
#define TIME_INTR       (TIMER_BASE + 0x34)
#define TIME_INTE       (TIMER_BASE + 0x38)
#define TIME_INTF       (TIMER_BASE + 0x3C)
#define TIME_INTS       (TIMER_BASE + 0x40)

#define RTC_BASE        0x4005C000
#define RTC_CLK_DIV     (RTC_BASE + 0x00)
#define RTC_SETUP_0     (RTC_BASE + 0x04)
#define RTC_SETUP_1     (RTC_BASE + 0x08)
#define RTC_CTRL        (RTC_BASE + 0x0C)
#define RTC_IRQ_SETUP_0 (RTC_BASE + 0x10)
#define RTC_IRQ_SETUP_1 (RTC_BASE + 0x14)
#define RTC_REG_1       (RTC_BASE + 0x18)
#define RTC_REG_0       (RTC_BASE + 0x1C)
#define RTC_RAW_INT     (RTC_BASE + 0x20)
#define RTC_INTE        (RTC_BASE + 0x24)
#define RTC_INTF        (RTC_BASE + 0x28)
#define RTC_INTS        (RTC_BASE + 0x2C)

#define ALARM_SIZE      0x04

#define ALARM0          0
#define ALARM1          1
#define ALARM2          2
#define ALARM3          3

void sleep_us(uint32_t delay, uint32_t alarm);

static inline void sleep_ms(uint32_t delay, uint32_t alarm) {
    sleep_us(delay * 1000, alarm);
}

static inline void busy_wait_us(uint32_t delay) {
    uint32_t start_time = mmio_read_32(TIME_L_READ);
    while(mmio_read_32(TIME_L_READ) - start_time < delay);
}

static inline void busy_wait_ms(uint32_t delay) {
    busy_wait_us(delay * 1000);
}
