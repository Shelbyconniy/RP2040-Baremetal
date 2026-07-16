#include <stdint.h>
#include "gpio.h"
#include "mmio.h"
#include "mpu.h"
#include "mailbox.h"
#include "interrupts.h"
#include "time.h"

extern uint32_t _stack1_bot;

#define MAILBOX_CMD_1   1
#define LED             25

void core1_main() {

    /* Protect stack1 */
    mpu_config(MPU_ENABLE | PRIVDEFENA);
    mpu_set_region(0, (uint32_t) &_stack1_bot, REGION_SIZE(5) | NO_ACCESS | NO_EXECUTE | EN_REGION);

    /* Enable ALARM1 in NVIC */
    NVIC_EnableIRQ(TIMER_IRQ_1);

    /* Inter-core synchronization */
    mailbox_sync_slave();

    while (1) {
        mailbox_read_blocking(); // wait a signal from Core 0
        gpio_clear(LED);        // Turn LED off
        sleep_ms(1000, ALARM1); // Wait for 1s
        mailbox_send_blocking(MAILBOX_CMD_1); // Send a signal to Core 0
    }

}
