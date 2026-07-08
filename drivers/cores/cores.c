#include "cores.h"
#include "mailbox.h"
#include "interrupts.h"

void core1_start(uint32_t *vector_table, uint32_t *stack_pointer, void (*func_ptr) (void)) {

    uint32_t init_buf[] = 
    {
    0,
    0,
    1,
    (uint32_t) vector_table,
    (uint32_t) stack_pointer,
    (uint32_t) func_ptr,
    };

    uint32_t seq_ind = 0;
    uint32_t mbox_rx;
    while (seq_ind < 6) {

        if (init_buf[seq_ind] == 0) {
            mailbox_clear();
            __SEV();
        }

        mailbox_send_blocking(init_buf[seq_ind]);
        mbox_rx = mailbox_read_blocking(); // wait echo
        seq_ind = (mbox_rx == init_buf[seq_ind]) ? seq_ind + 1 : 0; // confirm message
    }
}
