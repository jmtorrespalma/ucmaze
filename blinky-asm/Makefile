#   Copyright (C) 2018 Juan Manuel Torres Palma <j.m.torrespalma@gmail.com>
#   This file is part of ucmaze.
#
#   ucmaze is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   ucmaze is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with ucmaze.  If not, see <http://www.gnu.org/licenses/>.

# Makefile to build blinky-asm

# Define cross toolchain prefix
CROSS_COMPILE := arm-none-eabi
CPU := arm7tdmi

TARGET := blinky-asm

# Set all the flags
CFLAGS = -mcpu=$(CPU)
CFLAGS += -Wall -Wcast-align -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -g -0g
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding

ASFLAGS = -mcpu=$(CPU)
ASFLAGS += -x assembler-with-cpp

LDFLAGS = -nostartfiles -nostdlib
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -lgcc
LDFLAGS +=-Tat91sam7s64.ld

# Define programs and commands.
CC = $(CROSS_COMPILE)-gcc
CPP = $(CROSS_COMPILE)-g++
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP = $(CROSS_COMPILE)-objdump
SIZE = $(CROSS_COMPILE)-size
NM = $(CROSS_COMPILE)-nm
OPENOCD = openocd

# Program files
SRC := $(wildcard *.c)
ASRC += $(wildcard *.S)
OBJ := $(SRC:.c=.o) $(ASRC:.S=.o)
INC := $(wildcard *.h)

# Default targets
%.o: %.c
	$(CC) -c $(CFLAGS) -I. $< -o $@

%.o: %.S
	$(CC) -c $(ASFLAGS) -I. $< -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

# Final target
$(TARGET).elf: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Flash with openocd
flash: $(TARGET).bin
	$(OPENOCD) -f openocd-flash.cfg

.PHONY: all default clean format

default: all

all: $(TARGET).bin

format:
	indent -linux $(SRC)

clean:
	rm -rf $(OBJ) *.elf *.bin *.map *~
