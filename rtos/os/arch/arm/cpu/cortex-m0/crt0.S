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

	.syntax unified
	.thumb

	.section .isr_vect
	.word _kstack_top
	.word _start
	.word _startup_handler
	.word _startup_handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word _syscall_handler
	.word 0
	.word 0
	.word _context_switch
	.word ticker_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word 0
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word 0
	.word _startup_handler


/*
 * Execution starts in thread mode (usermode) and using kernel stack (msp), so
 * we need to adjust that and jump to the kernel, but C runtime needs to be set
 * before jumping in.
 * - Set usermode stack (psp)
 * - Copy data section to RAM
 * - Initialize bss
 * - Call platform initialization (clock, memory remap)
 * - Jump to kernel.
 */
	.section .init, "ax"
	.global _start
	.func   _start
	.thumb_func
_start:
	# Initialize usermode stack
	ldr	r0, =_ustack_top
	msr	psp, r0
	movs	r0, #2
	msr	control, r0

	# Copy data to RAM
	ldr	r0, =_data_rom
	ldr	r1, =_data_start
	ldr	r2, =_data_end
0:
	cmp	r1, r2
	beq	_clear_bss
	ldr	r3, [r0]
	str	r3, [r1]
	adds	r0, r0, #4
	adds	r1, r1, #4
	b	0b

_clear_bss:
	ldr	r0, =_bss_start
	ldr	r1, =_bss_end
	movs	r2, #0
1:
	cmp	r0, r1
	beq	2f
	str	r2, [r0]
	adds	r0, r0, #4
	b	1b

2:
	bl	soc_init /* Does stack init, remapping, etc */
	bl	cpu_init
	bl	board_init /* Initialize board components */
	bl	_system_start
	b	.

        .size   _start, . - _start
        .endfunc

/*
 * This function is called if an interrupt occurs in early boot, which is
 * nothing but a disaster.
 */
	.global _startup_handler
	.func   _startup_handler
	.thumb_func
        .size   _startup_handler, . - _startup_handler
_startup_handler:
	b	_startup_handler
        .endfunc
