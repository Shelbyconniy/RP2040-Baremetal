#include <stdint.h>
#include "mmio.h"
#include "resets.h"
#include "clocks.h"
#include "interrupts.h"
#include "time.h"
#include "gpio.h"
#include "mpu.h"

/* vector table pointers */
extern uint32_t _vector_src, _vector_st, _vector_end;
// extern uint32_t _vector1_src, _vector1_st, _vector1_end;

/* data pointers */
extern uint32_t _data_src, _data_st, _data_end;

/* bss pointers */
extern uint32_t _bss_st, _bss_end;

/* heap pointers */
extern uint32_t _heap_st, _heap_end;

/* stack pointers */
extern uint32_t _stack0_top, _stack0_bot; // 0x20040000
extern uint32_t _stack1_top, _stack1_bot; // 0x20041000

extern int core0_main();


void Default_Handler()  __attribute__((noreturn));
void Reset_Handler()    __attribute__((noreturn));

void NMI_Handler            () __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler      () __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler         () __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler         () __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler        () __attribute__((weak, alias("Default_Handler")));

void Timer0_IRQHandler      () __attribute__((weak, alias("Default_Handler")));
void Timer1_IRQHandler      () __attribute__((weak, alias("Default_Handler")));
void Timer2_IRQHandler      () __attribute__((weak, alias("Default_Handler")));
void Timer3_IRQHandler      () __attribute__((weak, alias("Default_Handler")));
void PWM_IRQHandler         () __attribute__((weak, alias("Default_Handler")));
void USB_IRQHandler         () __attribute__((weak, alias("Default_Handler")));
void XIP_IRQHandler         () __attribute__((weak, alias("Default_Handler")));
void PIO0_IRQHandler_0      () __attribute__((weak, alias("Default_Handler")));
void PIO0_IRQHandler_1      () __attribute__((weak, alias("Default_Handler")));
void PIO1_IRQHandler_0      () __attribute__((weak, alias("Default_Handler")));
void PIO1_IRQHandler_1      () __attribute__((weak, alias("Default_Handler")));
void DMA_IRQHandler_0       () __attribute__((weak, alias("Default_Handler")));
void DMA_IRQHandler_1       () __attribute__((weak, alias("Default_Handler")));
void GPIO0_IRQHandler       () __attribute__((weak, alias("Default_Handler")));
void QSPI_IRQHandler        () __attribute__((weak, alias("Default_Handler")));
void Mailbox0_IRQHandler    () __attribute__((weak, alias("Default_Handler")));
void Mailbox1_IRQHandler    () __attribute__((weak, alias("Default_Handler")));
void Clock_IRQHandler       () __attribute__((weak, alias("Default_Handler")));
void SPI0_IRQHandler        () __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler        () __attribute__((weak, alias("Default_Handler")));
void UART0_IRQHandler       () __attribute__((weak, alias("Default_Handler")));
void UART1_IRQHandler       () __attribute__((weak, alias("Default_Handler")));
void ADC_IRQHandler         () __attribute__((weak, alias("Default_Handler")));
void I2C0_IRQHandler        () __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler        () __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler         () __attribute__((weak, alias("Default_Handler")));

typedef void (*vectFunc) (void);

vectFunc vector_table[] __attribute__((section(".vector"))) =
    {
        (vectFunc)(&_stack0_top),
        Reset_Handler,
        NMI_Handler,
        HardFault_Handler,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        SVCall_Handler,
        0,
        0,
        PendSV_Handler,
        SysTick_Handler,
        Timer0_IRQHandler,
        Timer1_IRQHandler,
        Timer2_IRQHandler,
        Timer3_IRQHandler,
        PWM_IRQHandler,
        USB_IRQHandler,
        XIP_IRQHandler,
        PIO0_IRQHandler_0,
        PIO0_IRQHandler_1,
        PIO1_IRQHandler_0,
        PIO1_IRQHandler_1,
        DMA_IRQHandler_0,
        DMA_IRQHandler_1,
        GPIO0_IRQHandler,
        QSPI_IRQHandler,
        Mailbox0_IRQHandler,
        Mailbox1_IRQHandler,
        Clock_IRQHandler,
        SPI0_IRQHandler,
        SPI1_IRQHandler,
        UART0_IRQHandler,
        UART1_IRQHandler,
        ADC_IRQHandler,
        I2C0_IRQHandler,
        I2C1_IRQHandler,
        RTC_IRQHandler,
        0,
        0,
        0,
        0,
        0,
        0
    };

void Reset_Handler() {

    // 1. Configure XOSC and PLL
    
    // 1.1 Set Startup Delay
    // 47 for 12 MHz
    setBits(XOSC_STARTUP, 47);
    // 1.2 Enable XOSC
    mmio_write_32(XOSC_CTRL, XOSC_CTRL_ENABLE);
    // 1.3 Wait until stable
    while(!(mmio_read_32(XOSC_STATUS) & XOSC_STATUS_STABLE));

    // 1.4 Get PLL out of reset state
    enable_devices(PLL_SYS_RST);

    // Target frequency - 125 MHz

    // 1.5 Configure reference clock divider (already = 1)

    // 1.6 Configure feedback divider (= 125)
    setBits(PLL_SYS_FBDIV_INT, 125);

    // 1.7 Turn on main power and VCO
    clearBits(PLL_SYS_POWER, VCO | PLL);

    // 1.8 Wait for VCO to lock
    while(!(mmio_read_32(PLL_SYS_CONTROL_STATUS) & PLL_LOCK));

    // 1.9 Set up post dividers (6 and 2) and turn them on
    setBits(PLL_SYS_PRIM, POSTDIV1(6) | POSTDIV2(2));
    clearBits(PLL_SYS_POWER, POSTDIV);

    // 1.10 Set up clk_ref
    setBits(CLK_REF_CTRL, 0x2);

    // 1.11 Wait until stable
    while(!(mmio_read_32(CLK_REF_SEL) & (1 << 2)));

    // 1.12 Set up clk_sys
    setBits(CLK_SYS_CTRL, 0x1);
    // 1.13 Wait until stable
    while(!(mmio_read_32(CLK_SYS_SEL) & (1 << 1)));

    // 1.14 Enable clk_peri
    setBits(CLK_PERI_CTRL, (1 << 11));

    // 1.15 Set tick generator divider
    clearBits(WATCHDOG_TICK, 0x1FF); // [8:0]
    setBits(WATCHDOG_TICK, (12 << 0));

    // 1.16 Disable ROSC
    mmio_write_32(ROSC_CTRL, ROSC_CTRL_DISABLE);

    // 2. Copy vector table to SRAM
    //      and change VTOR value

    uint32_t *src = &_vector_src;
    uint32_t *st = &_vector_st;

    while (st < &_vector_end)
        *st++ = *src++;

    st = &_vector_st;
    mmio_write_32(VTOR_ADDR, (uint32_t) st);

    // 3. Copy .data to SRAM
    src = &_data_src;
    st = &_data_st;

    while (st < &_data_end)
        *st++ = *src++;

    // 4. Init .bss at SRAM
    st = &_bss_st;
    while (st < &_bss_end)
        *st++ = 0;

    __DSB();
    __ISB();
    
    // 5. Protect stack0 via MPU
    mpu_config(MPU_ENABLE | PRIVDEFENA);
    // 2^5 = 32 (bytes)
    mpu_set_region(0, (uint32_t) &_stack0_bot, REGION_SIZE(5) | NO_ACCESS | NO_EXECUTE | EN_REGION);

    // 6. Call main()
    core0_main();

    while(1);
}

void Default_Handler() {
    while(1);
}
