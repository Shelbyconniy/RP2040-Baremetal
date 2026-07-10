
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

.equ size_8,        (7 << 16)

.equ ctrl0_set,     (3 << 8) | (31 << 16) | (2 << 21)
.equ spi_set1,      (8 << 2) | (2 << 8) | (4 << 11) | (1 << 0)
.equ spi_set2,      (8 << 2) | (0 << 8) | (0xA0 << 24) | (4 << 11) | (2 << 0)       

.global boot2
.type boot2, %function
.thumb_func
boot2:

    @ QSPI init
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



    @ Disable SSI to configure
    movs r1, #0
    ldr r0, =ssi_base
    str r1, [r0, #0x08]

    @ Set clock divider to 4
    movs r1, #4
    str r1, [r0, #0x14]

    @ Set 8 clocks per data frame (7 << 16)
    @ Written value increases by 1
    ldr r1, =size_8
    str r1, [r0]

    @ Enable SSI
    movs r1, #1
    str r1, [r0, #0x08]
    
    @ Request the SR1 value
    movs r1, #5
    str r1, [r0, #0x60]

    @ Request the SR2 value
    movs r1, #0x35
    str r1, [r0, #0x60]

    movs r4, #1
    movs r5, #4


.wait_ssi_busy_1:

    @ Check SSI busy flag
    @ (1 << 2) bit in SSI Status Reg
    ldr r1, [r0, #0x28]
    tst r1, r5
    beq .wait_ssi_busy_1

    @ Check if TX FIFO is not empty
    @ (1 << 0) bit in SSI Status Reg
    tst r1, r4
    bne .wait_ssi_busy_1
    
    @ Read Flash SR1
    ldr r2, [r0, #0x60]

    @ Read Flash SR2
    ldr r3, [r0, #0x60]

    @ Check Quad-Enable bit
    movs r1, #2
    ands r1, r3
    movs r4, #2
    
    tst r1, r4
    bne .continue

    @ If Quad-Enable bit is not set

    @ Send write enable cmd
    movs r1, #0x06
    str r1, [r0, #0x60]

    @ Request for write to SR1 and SR2
    movs r1, #0x01
    str r1, [r0, #0x60]

    @ Write previous SR1 value back
    str r2, [r0, #0x60]

    @ Write previous SR2 value back with Quad-Enable bit (1 << 1)
    orrs r3, r4
    str r3, [r0, #0x60]

    @ Send write disable cmd
    movs r1, #0x04
    str r1, [r0, #0x60]

.continue:

    @ Prepare for dummy read

    @ Disable SSI to configure (again)
    movs r2, #0
    movs r3, #1
    str r2, [r0, #0x08]

    @ Configure SSI:
    @   - Quad-IO mode (2 << 21)
    @   - EEPROM read mode: Write, then Read (3 << 8)
    @   - 32 clocks per data frame (31 << 16)

    ldr r1, =ctrl0_set
    str r1, [r0]

    @ Configure SPI:
    @   - 32-bit address (8 << 2, 4-bit increments)
    @   - 8-bit instruction length (2 << 8)
    @   - 4 wait cycles (4 << 11)
    @   - Command in standard SPI format, address in format specified by FRF (1 << 0)
    ldr r0, =ssi_spi
    ldr r1, =spi_set1
    str r1, [r0]

    @ Enable SSI
    ldr r0, =ssi_base
    str r3, [r0, #0x08]

    @ Perform a dummy read

    @ Send "Fast Read Quad-IO" cmd
    movs r1, #0xEB
    str r1, [r0, #0x60]

    @ Perform Continuous Read (XIP)
    @ In Continuous Read mode it will be enough to send the address to receive data from Flash
    movs r1, #0xA0
    str r1, [r0, #0x60]

    movs r4, #1
    movs r5, #4

.wait_ssi_busy_2:

    @ Check SSI busy flag
    ldr r1, [r0, #0x28]
    tst r1, r5
    beq .wait_ssi_busy_2
    
    @ Check if TX FIFO is not empty
    tst r1, r4
    bne .wait_ssi_busy_2

    @ Disable SSI one more time
    str r2, [r0, #0x08]

    @ Final SPI configure:
    @   - 32-bit address (8 << 2)
    @   - 0xA0 for Continuous Read (0xA0 << 24)
    @   - 4 wait cycles (4 << 11)
    @   - Command and address both in format specified by FRF (2 << 0)
    ldr r0, =ssi_spi
    ldr r1, =spi_set2
    str r1, [r0]

    @ Enable SSI
    ldr r0, =ssi_base
    str r3, [r0, #0x08]
    
    @ Set SP
    ldr r0, =_stack0_top
    msr msp, r0

    @ Call Reset_Handler
    ldr r0, =Reset_Handler
    blx r0

    @ End of boot2




