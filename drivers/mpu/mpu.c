
#include "mpu.h"
#include "mmio.h"

volatile uint32_t mpu_ctrl;

void mpu_config(uint32_t ctrl_flags) {
    __DMB();

    mpu_ctrl = ctrl_flags;
    mmio_write_32(MPU_CTRL, mpu_ctrl);

    __DSB();
    __ISB();
}

void mpu_set_region(uint8_t region, uint32_t addr, uint32_t region_flags) {
    __DMB();

    mmio_write_32(MPU_RBAR, VALID | REGION(region) | REGION_ADDR(addr));
    mmio_write_32(MPU_RASR, region_flags);

    __DSB();
    __ISB();
}