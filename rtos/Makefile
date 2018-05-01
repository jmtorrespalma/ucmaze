# Copyright (C) 2018 Juan Manuel Torres Palma <j.m.torrespalma@gmail.com>
# This file is part of ucmaze.
#
# ucmaze is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ucmaze is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ucmaze.  If not, see <http://www.gnu.org/licenses/>.

# Makefile template for embedded builds

# Define project specific variables
CROSS_COMPILE := arm-none-eabi
ARCH := arm
CPU := cortex-m0
SOC := stm32f051
BOARD := stm32f0discovery
KERNEL := ucmaze-os
TARGET := blinky-parallel
VERSION := v0.1

# Directories
ROOT_DIR := .
OS_DIR := $(ROOT_DIR)/os
APP_DIR := $(ROOT_DIR)/app
KSRCD := include sched init util drivers dev io syscall
KERNEL_DIRS := $(foreach dir, $(KSRCD), $(OS_DIR)/$(dir))
DEBUG_DIR := $(OS_DIR)/debug
BOARD_DIR := $(OS_DIR)/arch/$(ARCH)/board/$(BOARD)
CPU_DIR := $(OS_DIR)/arch/$(ARCH)/cpu/$(CPU)
SOC_DIR := $(OS_DIR)/arch/$(ARCH)/soc/$(SOC)
K_SRC_DIRS := $(KERNEL_DIRS) $(BOARD_DIR) $(CPU_DIR) $(SOC_DIR)
A_SRC_DIRS := $(APP_DIR)

# Other files
LDSCRIPT := $(SOC_DIR)/linker.ld
CONNECT_CFG := board/$(BOARD).cfg
FLASH_CFG := $(BOARD_DIR)/flash.cfg
DEBUG_SCRIPT := $(DEBUG_DIR)/debug.gdb

# Set all the flags
CFLAGS = -mcpu=$(CPU)
CFLAGS += -Wall -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -g -Og
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding
CFLAGS += -I$(OS_DIR) -I$(OS_DIR)/include

ASFLAGS = -mcpu=$(CPU)
ASFLAGS += -g
ASFLAGS += -x assembler-with-cpp

LDFLAGS = -mcpu=$(CPU)
LDFLAGS += -nostartfiles -nostdlib
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -lgcc
LDFLAGS += -T$(LDSCRIPT)

# Define programs and commands.
CC := $(CROSS_COMPILE)-gcc
CPP := $(CROSS_COMPILE)-g++
AR := $(CROSS_COMPILE)-ar
OBJCOPY := $(CROSS_COMPILE)-objcopy
OBJDUMP := $(CROSS_COMPILE)-objdump
SIZE := $(CROSS_COMPILE)-size
NM := $(CROSS_COMPILE)-nm
GDB := $(CROSS_COMPILE)-gdb
OPENOCD := openocd
RM := rm -f
KILL := pkill
TAR := tar
INDENT := indent

# Kernel files
K_SRC := $(shell find $(K_SRC_DIRS) -name "*.c")
K_INC := $(shell find $(K_SRC_DIRS) -name "*.h")
K_ASRC := $(shell find $(K_SRC_DIRS) -name "*.S")
K_OBJ := $(K_SRC:.c=.o) $(K_ASRC:.S=.o)
KLIB := $(KERNEL).a

# App files
A_SRC := $(shell find $(A_SRC_DIRS) -name "*.c")
A_INC := $(shell find $(A_SRC_DIRS) -name "*.h")
A_ASRC := $(shell find $(A_SRC_DIRS) -name "*.S")
A_OBJ := $(A_SRC:.c=.o) $(A_ASRC:.S=.o)

# Implicit rules
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) -c $(ASFLAGS) $< -o $@

%.elf: $(A_OBJ) $(KLIB)
	$(CC) -o $@ $^ $(LDFLAGS)

%.bin: %.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@

# Avoid removing intermediate files
.PRECIOUS: %.o %.elf %.a

# Non file rules
.PHONY: all flash debug size format clean

# Default rule
all: $(TARGET).bin

# Kernel static object
$(KLIB): $(K_OBJ)
	$(AR) rcs $@ $^

# Flash with openocd
flash: $(TARGET).bin
	$(OPENOCD) -c "set PROGRAM \"$(TARGET).bin\"" -f $(CONNECT_CFG) \
		-f $(FLASH_CFG)

# Start debugging session
debug: $(TARGET).elf
	$(OPENOCD) -f $(CONNECT_CFG) -c "init" -c "halt" &
	$(GDB) -x $(DEBUG_SCRIPT) $<
	$(KILL) $(OPENOCD)

# Show executable size
size: $(TARGET).elf
	$(SIZE) $<

# Make C code look pretty
format:
	$(INDENT) -linux $(SRC)

# Clean up temporary files
clean:
	$(RM) $(K_OBJ) $(A_OBJ) *.a *.elf *.bin *.map *.tgz *~

# Package distribution
BUILD_DIST := $(SRC) $(ASRC) $(INC) $(LDSCRIPT) Makefile
EXTRA_DIST := $(CONNECT_CFG) $(FLASH_CFG) $(DEBUG_SCRIPT)
dist:
	$(TAR) -czvf $(TARGET)-$(VERSION).tgz $(BUILD_DIST) $(EXTRA_DIST)
