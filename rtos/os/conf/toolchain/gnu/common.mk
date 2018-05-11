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

# Parameters and tools understood by the GNU toolchain

# Include architecture dependent defines
include os/conf/toolchain/$(TOOLCHAIN)/$(ARCH).mk

# Files
LDSCRIPT ?= $(SOC_DIR)/linker.ld

# Set all the flags
CFLAGS += -Wall -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -g -Og
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding
CFLAGS += -I$(OS_DIR) -I$(OS_DIR)/include

ASFLAGS += -g
ASFLAGS += -x assembler-with-cpp

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
RM := rm -f
KILL := pkill
TAR := tar
INDENT := indent
