TARGET = firmware
BUILD_DIR = build

CORE      := ../core
CMSIS     := $(CORE)/cmsis
STARTUP   := $(CORE)/startup
DEVICE    := $(CMSIS)/device

# Required files
SYSTEM_SRC   := $(DEVICE)/system_stm32f4xx.c
STARTUP_SRC  := $(STARTUP)/startup_stm32f411xe.s
LDSCRIPT     := $(STARTUP)/STM32F411RETx_FLASH.ld

# Project C files
PROJECT_SRC = $(shell find . -name '*.c')

# Add system + startup
SRC = $(PROJECT_SRC) $(SYSTEM_SRC)
ASM = $(STARTUP_SRC)

PREFIX = arm-none-eabi-
CC  = $(PREFIX)gcc
AS  = $(PREFIX)gcc -x assembler-with-cpp
CP  = $(PREFIX)objcopy
SZ  = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=soft

CFLAGS = $(MCU) -Wall -O0 -g \
    -DSTM32F411xE \
    -fdata-sections -ffunction-sections \
    -I$(CMSIS)/core \
    -I$(DEVICE) \
    -Iinclude

ASFLAGS = $(MCU) -g -Wall

LDFLAGS = $(MCU) -T$(LDSCRIPT) \
    -specs=nosys.specs -specs=nano.specs \
    -Wl,--gc-sections \
    -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))
OBJ += $(BUILD_DIR)/startup.o

vpath %.c $(sort $(dir $(SRC)))
vpath %.s $(sort $(dir $(ASM)))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC $<"
	@$(CC) -c $(CFLAGS) $< -o $@ -MMD -MP -MF"$(@:%.o=%.d)"

$(BUILD_DIR)/startup.o: $(STARTUP_SRC)
	@echo "AS $<"
	@$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJ)
	@echo "LD $@"
	@$(CC) $(OBJ) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x8000000

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean flash
