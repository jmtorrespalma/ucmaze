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

# The whole buildsystem is simply based in make since it's very fast, almost
# standard and easy to use.
#
# This Makefile is expecting for variables APP and BOARD to be set in the
# environment and takes care of building the appropriate source files.
#
# For a given board, there must be a BOARD.mk file that defines variables such
# as CPU, ARCH, etc. In some special cases, some extra BOARD-rules.mk is
# required if that target has any special rule that is not covered by all the
# common standard rules defined here.

ifndef BOARD
BOARD := sam7-p64
endif

ifndef APP
APP := single-task
endif

ifndef TOOLCHAIN
TOOLCHAIN := gnu
endif

ifndef FLASH_TOOL
FLASH_TOOL := openocd
endif

include os/conf/board/$(BOARD).mk

# Define project specific variables
KERNEL := ucmaze-os
TARGET := $(APP)
VERSION := v0.1

# Directories
ROOT_DIR := .
OS_DIR := $(ROOT_DIR)/os
APP_DIR := $(ROOT_DIR)/$(APP)
KSRCD := include sched init util drivers dev io syscall
KERNEL_DIRS := $(foreach dir, $(KSRCD), $(OS_DIR)/$(dir))
DEBUG_DIR := $(OS_DIR)/debug
BOARD_DIR := $(OS_DIR)/arch/$(ARCH)/board/$(BOARD)
CPU_DIR := $(OS_DIR)/arch/$(ARCH)/cpu/$(CPU)
SOC_DIR := $(OS_DIR)/arch/$(ARCH)/soc/$(SOC)
K_SRC_DIRS := $(KERNEL_DIRS) $(BOARD_DIR) $(CPU_DIR) $(SOC_DIR)
A_SRC_DIRS := $(APP_DIR)

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

# Include toolchain definitions
include os/conf/toolchain/$(TOOLCHAIN).mk

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

# Flash and debugging is tool dependent, should define the rules:
# - flash: to burn the compiled app into the board.
# - debug: to connect and debug the application.
include os/conf/flash/$(FLASH_TOOL).mk

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
