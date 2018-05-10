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

# For debugging, openocd requires gdb.

# These here could have been defined by the target configuration previously.
CONNECT_CFG ?= $(BOARD_DIR)/connect.cfg
FLASH_CFG ?= $(BOARD_DIR)/flash.cfg
DEBUG_SCRIPT ?= $(DEBUG_DIR)/debug.gdb

OPENOCD := openocd

# Flash with openocd
flash: $(TARGET).bin
	$(OPENOCD) -c "set PROGRAM \"$(TARGET).bin\"" -f $(CONNECT_CFG) \
		-f $(FLASH_CFG)

# Start debugging session
debug: $(TARGET).elf
	$(OPENOCD) -f $(CONNECT_CFG) -c "init" -c "halt" &
	$(GDB) -x $(DEBUG_SCRIPT) $<
	$(KILL) $(OPENOCD)
