#include <stdint.h>
#include "gpio.h"
#include "resets.h"
#include "interrupts.h"
#include "mailbox.h"
#include "time.h"
#include "mmio.h"
#include "cores.h"

#define LED             25
#define GPIO_FUNC       5
#define MAILBOX_CMD_0   0

extern void core1_main();


void core0_main()
{
    /* Taking devices out of reset state */
    enable_devices(GPIO_RST | TIMER_RST);

    /* Enable ALARM0 in NVIC */
    NVIC_EnableIRQ(TIMER_IRQ_0);

    /* GP25 -> GPIO-mode */
    pinmux_set(LED, GPIO_FUNC);

    /* GP25 -> Output */
    gpio_mode_output(LED);

    /* Init core 1 */
    core1_start(VECTOR_TABLE_DEFAULT, STACK1_DEFAULT, core1_main);

    /* Inter-core synchronization*/
    mailbox_sync_master();

    while (1) {
        gpio_set(LED); // Turn LED on
        sleep_ms(1000, ALARM0); // Wait for 1s
        mailbox_send_blocking(MAILBOX_CMD_0); // Send a signal to Core 1
        mailbox_read_blocking(); // Wait a signal from Core 1
    }

}

// void core0_main()
// {
//     enable_devices(GPIO_RST | TIMER_RST);
	
//     NVIC_EnableIRQ(TIMER_IRQ_0);

//     pinmux_set(LED, GPIO_FUNC);
//     gpio_mode_output(LED);

//     while (1) {
//         sleep_ms(1000, ALARM0);
//         gpio_invert(LED);
//     }
// }
