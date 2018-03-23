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
CPU := arm926ej-s
BOARD := integratorcp
TARGET := hello-qemu
VERSION := v0.1

# Set all the flags
CFLAGS = -mcpu=$(CPU)
CFLAGS += -Wall -Wcast-align -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -g -0g
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding

ASFLAGS = -mcpu=$(CPU)
ASFLAGS += -g
ASFLAGS += -x assembler-with-cpp

LDSCRIPT := qemu-linker.ld
LDFLAGS = -nostartfiles -nostdlib
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -lgcc
LDFLAGS +=-T $(LDSCRIPT)

# Define programs and commands.
CC := $(CROSS_COMPILE)-gcc
CPP := $(CROSS_COMPILE)-g++
OBJCOPY := $(CROSS_COMPILE)-objcopy
OBJDUMP := $(CROSS_COMPILE)-objdump
SIZE := $(CROSS_COMPILE)-size
NM := $(CROSS_COMPILE)-nm
GDB := $(CROSS_COMPILE)-gdb
OPENOCD := openocd
QEMU := qemu-system-arm
RM := rm -f
KILL := pkill
TAR := tar
INDENT := indent

# Program files
SRC := $(wildcard *.c)
ASRC += $(wildcard *.S)
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
	$(OBJCOPY) -O binary $< $@

# Avoid removing intermediate files
.PRECIOUS: %.o %.elf

# Default rule
.PHONY: all run debug format clean
all: $(TARGET).bin

# Flash with openocd
run: $(TARGET).bin
	@echo "Press Ctrl+a, x to exit qemu"
	$(QEMU) -M $(BOARD) -m 128M -nographic -kernel $^

# Start debugging session
DEBUG_SCRIPT := debug.gdb
debug: $(TARGET).bin $(TARGET).elf
	$(QEMU) -M $(BOARD) -m 128M -nographic -s -S -kernel $< &
	$(GDB) -x $(DEBUG_SCRIPT) $(TARGET).elf
	$(KILL) $(QEMU)

# Make C code look pretty
format:
	$(INDENT) -linux $(SRC)

# Clean up temporary files
clean:
	$(RM) $(OBJ) *.elf *.bin *.map *.tgz *~

# Package distribution
BUILD_DIST := $(SRC) $(ASRC) $(INC) $(LDSCRIPT) Makefile
EXTRA_DIST := $(CONNECT_CFG) $(FLASH_CFG) $(DEBUG_SCRIPT)
dist: clean
	$(TAR) -czvf $(TARGET)-$(VERSION).tgz $(BUILD_DIST) $(EXTRA_DIST)
