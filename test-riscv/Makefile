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
CROSS_COMPILE := riscv64-unknown-elf
ARCH := rv32imac
ABI := ilp32
TARGET := riscv32-test
VERSION := v0.1
ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

# Set all the flags
CFLAGS = -march=$(ARCH) -mabi=$(ABI)
CFLAGS += -Wall -Wcast-align -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -g -Og
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding

ASFLAGS = -march=$(ARCH) -mabi=$(ABI)
ASFLAGS += -g
ASFLAGS += -x assembler-with-cpp

LDSCRIPT := linker.ld
LDFLAGS = -march=$(ARCH) -mabi=$(ABI)
LDFLAGS += -nostartfiles -nostdlib
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -lgcc
LDFLAGS += -T$(LDSCRIPT)

# Define programs and commands.
CC := $(CROSS_COMPILE)-gcc
CPP := $(CROSS_COMPILE)-g++
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

# Program files
SRC := $(wildcard *.c)
ASRC := $(wildcard *.S) $(wildcard **/*.S)
OBJ := $(SRC:.c=.o) $(ASRC:.S=.o)
INC := $(wildcard *.h)

# Implicit rules
%.o: %.c
	$(CC) -c $(CFLAGS) -I. $< -o $@

%.o: %.S
	$(CC) -c $(ASFLAGS) -I. $< -o $@

%.elf: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.bin: %.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@

# Avoid removing intermediate files
.PRECIOUS: %.o %.elf

# Default rule
.PHONY: all flash debug size format clean
all: $(TARGET).bin

# Flash with openocd
CONNECT_CFG := connect.cfg
FLASH_CFG := flash.cfg
flash: $(TARGET).bin
	$(OPENOCD) -f $(CONNECT_CFG) -f $(FLASH_CFG)

# Start debugging session
DEBUG_SCRIPT := debug.gdb
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
	$(RM) $(OBJ) *.elf *.bin *.map *.tgz *~

# Package distribution
BUILD_DIST := $(SRC) $(ASRC) $(INC) $(LDSCRIPT) Makefile
EXTRA_DIST := $(CONNECT_CFG) $(FLASH_CFG) $(DEBUG_SCRIPT)
dist:
	$(TAR) -czvf $(TARGET)-$(VERSION).tgz $(BUILD_DIST) $(EXTRA_DIST)
