# path to your global libopencm3 install
OPENCM3_DIR = ~/dev/libopencm3

# target MCU family
DEVICE = stm32f4
CHIP = stm32f411re  # example, adjust to your board

# compiler setup
PREFIX = arm-none-eabi
CC = $(PREFIX)-gcc
LD = $(PREFIX)-gcc
OBJCOPY = $(PREFIX)-objcopy
SIZE = $(PREFIX)-size

# project setup
TARGET = main
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(SRC:.c=.o)))

CFLAGS = -O2 -g -Wall -Wextra -std=gnu99
CFLAGS += -I$(INC_DIR) -I$(OPENCM3_DIR)/include
CFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=soft

LDFLAGS = -L$(OPENCM3_DIR)/lib -lopencm3_$(DEVICE)
LDFLAGS += -Tlinker.ld -nostartfiles

# default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin size

# ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# compile object files into build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# link all objects into final ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

# convert ELF to BIN
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# show binary size
size: $(BUILD_DIR)/$(TARGET).elf
	$(SIZE) $<

# flash target using st-flash
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x08000000

# clean everything
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean flash size
