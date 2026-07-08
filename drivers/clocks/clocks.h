#include <stdint.h>

#define CLOCKS_BASE             0x40008000

#define CLK_REF_CTRL            (CLOCKS_BASE + 0x30)
//   [1:0] SRC
//        - 0x0 -> ROSC_CLKSRC_PH
//        - 0x1 -> CLKSRC_CLK_REF_AUX
//        - 0x2 -> XOSC_CLKSRC
#define CLK_REF_XOSC            (2 << 0)

#define CLK_REF_DIV             (CLOCKS_BASE + 0x34)
#define CLK_REF_SEL             (CLOCKS_BASE + 0x38)

#define CLK_SYS_CTRL            (CLOCKS_BASE + 0x3C)
//   [7:5] AUX SRC
//        - 0x0 -> CLKSRC_PLL_SYS (default)
//        - 0x1 -> CLKSRC_PLL_USB
//        - 0x2 -> ROSC_CLKSRC
//        - 0x3 -> XOSC_CLKSRC
//   [1:0] SRC
//        - 0x0 -> CLK_REF
//        - 0x1 -> CLKSRC_CLK_SYS_AUX

#define CLK_SYS_DIV             (CLOCKS_BASE + 0x40)
#define CLK_SYS_SEL             (CLOCKS_BASE + 0x44)

#define CLK_PERI_CTRL           (CLOCKS_BASE + 0x48)
//   [11] Enable
//   [10] Kill
//   [7:5] AUX SRC
//        - 0x0 -> CLK_SYS
//        - 0x1 -> CLKSRC_PLL_SYS
//        - 0x2 -> CLKSRC_PLL_USB
//        - 0x3 -> ROSC_CLKSRC_PH
//        - 0x4 -> XOSC_CLKSRC

#define CLK_PERI_SEL            (CLOCKS_BASE + 0x50)

/* ROSC ctrl */
#define ROSC_BASE               0x40060000
#define ROSC_CTRL               (ROSC_BASE + 0x00)
//   [23:12] Enable
//        - 0xD1E -> DISABLE
//        - 0xFAB -> ENABLE
#define ROSC_CTRL_DISABLE        (0xD1E << 12)
#define ROSC_CTRL_ENABLE         (0xFAB << 12)

/* XOSC ctrl */
#define XOSC_BASE               0x40024000
#define XOSC_CTRL               (XOSC_BASE + 0x00)
//   [23:12] Enable
//         - 0xD1E Enable
//         - 0xFAB Disable

#define XOSC_CTRL_DISABLE        (0xD1E << 12)
#define XOSC_CTRL_ENABLE         (0xFAB << 12)

#define XOSC_STATUS             (XOSC_BASE + 0x04)
//   [31] Stable // RO 
#define XOSC_STATUS_STABLE      (1 << 31)

#define XOSC_DORMANT            (XOSC_BASE + 0x08)

#define XOSC_STARTUP            (XOSC_BASE + 0x0C)
//   [13:0] Startup Delay

#define XOSC_COUNT              (XOSC_BASE + 0x1C)


/* PLL ctrl */
#define PLL_SYS_BASE            0x40028000

#define PLL_SYS_CONTROL_STATUS  (PLL_SYS_BASE + 0x00)
//   [31]   LOCK        // RO
//   [8]    BYPASS      // RW
//   [5:0]  REFDIV      // RW
#define PLL_LOCK                (1 << 31)

#define PLL_SYS_POWER           (PLL_SYS_BASE + 0x04)
// 1 is Disabled
//   [5] VCO            // RW
//   [3] POSTDIV        // RW
//   [0] PLL            // RW

#define VCO                     (1 << 5)
#define POSTDIV                 (1 << 3)
#define PLL                     (1 << 0)

#define PLL_SYS_FBDIV_INT       (PLL_SYS_BASE + 0x08)
//   [11:0] DIV         // RW

#define PLL_SYS_PRIM            (PLL_SYS_BASE + 0x0C)
//   [18:16] POSTDIV1   // RW
//   [14:12] POSTDIV2   // RW
#define POSTDIV1(X)             (X << 16)
#define POSTDIV2(X)             (X << 12)