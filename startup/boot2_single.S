
.syntax unified
.cpu cortex-m0plus
.thumb

.section .boot2, "ax"

.extern _stack0_top
.extern Reset_Handler

.equ qspi_base,     0x40020000

.equ sclk_mask,    ~(3 << 4 | 1 << 0) @ 1100 1110
.equ fast_8mA,      (2 << 4 | 1 << 0) @ 0x21 = 0010 0001

.equ sd_mask,      ~(3 << 4 | 3 << 0) @ 1100 1100

.equ ssi_base,      0x18000000
.equ ssi_spi,       0x180000F4

.equ ctrl0_set,     (3 << 8) | (31 << 16)
.equ spi_set1,      (6 << 2) | (2 << 8) | (0x03 << 24)   

.global boot2
.type boot2, %function
.thumb_func
boot2:

    @ qspi init
    @ Set 8mA, slew-rate fast

    @ SCLK
    ldr r0, =qspi_base
    ldr r1, [r0, #0x04]
    ldr r2, =sclk_mask
    ands r1, r2
    ldr r2, =fast_8mA
    orrs r1, r2
    str r1, [r0, #0x04]

    @SD0
    ldr r1, [r0, #0x08]
    ldr r2, =sd_mask
    ands r1, r2
    ldr r2, =fast_8mA
    orrs r1, r2
    str r1, [r0, #0x08]

    @SD1
    str r1, [r0, #0x0C]

    @SD2
    str r1, [r0, #0x10]

    @SD3
    str r1, [r0, #0x14]


    @ ssi init

    @ disable ssi
    movs r1, #0
    ldr r0, =ssi_base
    str r1, [r0, #0x08]

    @ set divisor
    movs r1, #4
    str r1, [r0, #0x14]

    @ set 32 clocks
    ldr r1, =ctrl0_set
    str r1, [r0]

    @ set spi
    ldr r0, =ssi_spi
    ldr r1, =spi_set1
    str r1, [r0]

    ldr r0, =ssi_base
    movs r1, #1
    str r1, [r0, #0x08]
    
    @ set SP
    ldr r0, =_stack0_top
    msr msp, r0

    @ call Reset_Handler
    ldr r0, =Reset_Handler
    blx r0
    b .
    @ end of boot2




