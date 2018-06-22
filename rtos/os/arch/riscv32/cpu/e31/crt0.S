/*
 * Copyright (C) 2018 Juan Manuel Torres Palma <j.m.torrespalma@gmail.com>
 * This file is part of ucmaze.
 *
 * ucmaze is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ucmaze is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ucmaze.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * We start running in some kind of bios mode, that need to explicitly call
 * kernel mode with _system_start.
 */

	.section .init, "ax"
	.global _start
	.func   _start
_start:
.option push
.option norelax
	la	gp, _global_pointer
.option pop
	la	sp, _ustack_top
	la	a0, _kstack_top
	csrw	mscratch, a0

	# Copy data to RAM
	la	a0, _data_rom
	la	a1, _data_start
	la	a2, _data_end
0:
	bgt	a1, a2, 1f
	lw	a3, (a0)
	sw	a3, (a1)
	addi	a0, a0, 4
	addi	a1, a1, 4
	j	0b

	# Set bss to zero
1:
	la	a0, _bss_start
	la	a1, _bss_end
2:
	bgt	a0, a1, 3f
	sw	x0, (a0)
	addi	a0, a0, 4
	j	2b

3:
	jal	soc_init
	jal	cpu_init
	jal	board_init
	jal	_system_start
	j	.

        .size   _start, . - _start
        .endfunc
