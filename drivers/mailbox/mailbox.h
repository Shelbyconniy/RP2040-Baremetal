#include <stdint.h>
#include "sys_reg.h"
#include "sio.h"
#include "mmio.h"

/* FIFO TX/RX buffers */
#define FIFO_TX_REG (SIO_BASE + 0x054) // WF
#define FIFO_RX_REG (SIO_BASE + 0x058) // RF
/* FIFO buffers are 8-words wide */
/* Writing in FIFO_TX_REG pushes 32-bit data (word) to another's core FIFO RX buffer (WF) */
/* Reading FIFO_RX_REG returns first word from current core's FIFO RX buffer and
    removes this word from FIFO (RF) */

/* FIFO status register */
#define FIFO_STATUS_REG (SIO_BASE + 0x050) // RO
/* 1: True, 0: False */
/* [0] RX-buffer is not empty */ 
/* [1] TX-buffer is not full */
/* [2] TX-buffer was written when full */
/* [3] RX-buffer was read when empty */

#define MAILBOX_MAGIC   0x53594E43 // "SYNC"

static inline uint32_t mailbox_read() {
    return mmio_read_32(FIFO_RX_REG);
}

static inline uint32_t mailbox_read_blocking() {
    while (!(mmio_read_32(FIFO_STATUS_REG) & (1 << 0)))
        __WFE();
    return mailbox_read();
}
static inline void mailbox_send(uint32_t data) {
    mmio_write_32(FIFO_TX_REG, data);
    __SEV();
}

static inline void mailbox_send_blocking(uint32_t data) {
    while (!(mmio_read_32(FIFO_STATUS_REG) & (1 << 1)));
    mmio_write_32(FIFO_TX_REG, data);
    __SEV();
}

static inline void mailbox_clear() {
    while (mmio_read_32(FIFO_STATUS_REG) & (1 << 0))
        mailbox_read();
}

static inline void mailbox_sync_master() {
    uint32_t primask_state = __save_PRIMASK();
    mailbox_send_blocking(MAILBOX_MAGIC);
    __DMB();
    while (mailbox_read_blocking() != MAILBOX_MAGIC);
    __set_PRIMASK(primask_state);
}

static inline void mailbox_sync_slave() {
    uint32_t primask_state = __save_PRIMASK();
    while (mailbox_read_blocking() != MAILBOX_MAGIC);
    mailbox_send_blocking(MAILBOX_MAGIC);
    __DMB();
    __set_PRIMASK(primask_state);
}