
GCC			:= arm-none-eabi-gcc
OBJDUMP 	:= arm-none-eabi-objdump
OBJCOPY		:= arm-none-eabi-objcopy
LD 			:= arm-none-eabi-ld

UF2			:= tools/uf2/uf2conv.py
CRC32		:= tools/add_crc32/crc32_rp2040.py

drivers_dir := $(wildcard drivers/*)
src_drivers := $(notdir $(wildcard drivers/*/*.c))
src_c		:= core0_main.c core1_main.c startup.c
src_c		+= $(src_drivers)
src_asm		:= boot2_QuadIO.S

BUILD_DIR	:= build
OUTPUT_DIR	:= $(BUILD_DIR)/output
ASMFLAGS	:= -mcpu=cortex-m0plus $(addprefix -I, $(drivers_dir)) -g
GCCFLAGS	:= -mcpu=cortex-m0plus $(addprefix -I, $(drivers_dir)) -g -O3 -nostdlib -ffreestanding
LDFLAGS		:= -Map=$(OUTPUT_DIR)/memory.map -T linker.ld -nostdlib

vpath %.S startup
vpath %.c startup $(drivers_dir)

objects		:= $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(src_c))) \
				$(addprefix $(BUILD_DIR)/, $(patsubst %.S, %.o, $(src_asm)))
deps		:= $(patsubst %.o, %.d, $(objects))


all: $(OUTPUT_DIR)/pico_fw.uf2

$(BUILD_DIR) $(OUTPUT_DIR):
	mkdir -p $@

$(OUTPUT_DIR)/pico_fw.uf2: $(OUTPUT_DIR)/pico_fw.bin
	python3 $(UF2) -b 0x10000000 -f 0xe48bff56 -c $< -o $@

$(OUTPUT_DIR)/pico_fw.bin: $(OUTPUT_DIR)/pico_fw_raw.elf
	$(OBJCOPY) -O binary $< $@
	python3 $(CRC32) $@

$(OUTPUT_DIR)/pico_fw_raw.elf: $(objects) | $(OUTPUT_DIR)
	$(LD) $(LDFLAGS) $(objects) -o $@
	$(OBJDUMP) -hSD $@ > $(OUTPUT_DIR)/rp2040.objdump

-include $(deps)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(GCC) $(GCCFLAGS) -MMD -MP -MF $(patsubst %.o, %.d, $@) -c $< -o $@

$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	$(GCC) $(ASMFLAGS) -MMD -MP -MF $(patsubst %.o, %.d, $@) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
