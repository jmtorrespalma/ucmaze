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

ENTRY(_start)
MEMORY
{
  ROM (rx)  : ORIGIN = 0x00100000, LENGTH = 0x00010000 /* 64K */
  RAM (rwx) : ORIGIN = 0x00200000, LENGTH = 0x00004000 /* 16K */
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vect))
		. = ALIGN(4);
		KEEP(*(.init))
		. = ALIGN(4);
		ucmaze-os.a:*(.text*)
		*(.text*)
		. = ALIGN(4);
	} > ROM

	.rodata :
	{
		*(.rodata*)
		. = ALIGN(4);
	} > ROM

	.data :
	{
		_data_rom = LOADADDR(.data);
		_data_start = .;
		*(.data*)
		. = ALIGN(4);
		_data_end = .;
	} > RAM AT > ROM

	.bss :
	{
		_bss_start = .;
		*(.bss*)
		*(COMMON*)
		. = ALIGN(4);
		_bss_end = .;
	} > RAM AT > ROM

	/*
	 * TODO: read sizes from conf.h
	 */
	_kstack_top = ORIGIN(RAM)+LENGTH(RAM);
	_fiq_stack_top = _kstack_top - 64;
	_irq_stack_top = _fiq_stack_top - 128;
	_und_stack_top = _irq_stack_top - 64;
	_abt_stack_top = _und_stack_top - 64;
	_ustack_top = _abt_stack_top - 64;
}
