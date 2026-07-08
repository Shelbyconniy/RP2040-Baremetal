#include "sys_reg.h"
#include "mmio.h"

#define MPU_TYPE        (CORTEX_BASE + 0xED90) // RO
#define MPU_CTRL        (CORTEX_BASE + 0xED94) // RW
//      [31:3]  Reserved
//      [2]     PRIVDEFENA
//      [1]     HFNMIENA
//      [0]     ENABLE
#define PRIVDEFENA      (1UL << 2)
#define HFNMIENA        (1UL << 1)
#define MPU_ENABLE      (1UL << 0)
#define MPU_DISABLE     (0UL << 0)

#define MPU_RNR         (CORTEX_BASE + 0xED98)
//      [31:4] Reserved
//      [3:0] Region number

#define MPU_RBAR        (CORTEX_BASE + 0xED9C)
//      [31:8] ADDR
//      [4] VALID
//          0 = Ignore REGION
//          1 = Update MPU_RNR to REGION
//      [3:0] REGION
#define REGION_ADDR(X)  (X & 0xFFFFFF00)
#define VALID           (1 << 4)
#define REGION(X)       (X << 0)

#define MPU_RASR    (CORTEX_BASE + 0xEDA0)
//      [28] Instruction access disable

//      [5:1] Size ("2 ^ Size" is region size)
//      [0] Enable
#define REGION_SIZE(X)  (X << 1)
#define NO_EXECUTE      (1UL << 28)
#define NO_ACCESS       0x000
#define RO_ACCESS
#define WO_ACCESS
#define WR_ACCESS
#define SHAREABLE       (1UL << 18)
#define CACHEABLE       (1UL << 17)
#define BUFFERABLE      (1UL << 16)
#define EN_REGION       (1UL << 0)

void mpu_config(uint32_t ctrl_flags);
void mpu_set_region(uint8_t region, uint32_t addr, uint32_t region_flags);

