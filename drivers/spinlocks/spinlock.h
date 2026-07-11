#include "sio.h"
#include "mmio.h"
#include "sys_reg.h"

#define SPINLOCK_STATUS_REG (SIO_BASE + 0x05C)

/* Spinlocks (0-31) registers */
#define SPINLOCK_BASE (SIO_BASE + 0x100) // RW
#define SPINLOCK_SIZE 0x04
/* Reading values: */
/* 0:       Spinlock is already locked */
/* Nonzero: Spinlock was succesfully claimed */

static inline uint32_t claim_spinlock_blocking(uint32_t number) {
    uint32_t primask_state = __save_PRIMASK();
    __disable_irq();
    while (!mmio_read_32(SPINLOCK_BASE + SPINLOCK_SIZE * number));
    __DMB();
    return primask_state;

}

static inline void free_spinlock(uint32_t number, uint32_t primask_state) {
    __DMB();
    mmio_write_32(SPINLOCK_BASE + SPINLOCK_SIZE * number, 1);
    __set_PRIMASK(primask_state);
}
