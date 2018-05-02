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

#include "arm-reg.h"

	.syntax unified
	.code 32

	.section .isr_vect
	ldr	pc, [pc, #0x18] /* Reset */
	ldr	pc, [pc, #0x18] /* Undefined instruction */
	ldr	pc, [pc, #0x18] /* SVC */
	ldr	pc, [pc, #0x18] /* Prefetch abort */
	ldr	pc, [pc, #0x18] /* Data abort */
	ldr	pc, [pc, #0x18] /* Reserved */
	ldr	pc, [pc, #0x18] /* IRQ */
	ldr	pc, [pc, #0x18] /* FIQ */
	.word _start
	.word _startup_handler
	.word _svc_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _irq_handler
	.word _startup_handler

/*
 * This cpu starts running in supervisor mode, in arm state. From there we must
 * do the following:
 * - Set all modes stack.
 * - Copy data section to RAM.
 * - Set bss section to zero.
 * - Call platform initialization (clock, memory remap).
 * - Jump to kernel.
 */
	.section .init, "ax"
	.global _start
	.func   _start
_start:
	/* Copy data to RAM */
	ldr	r0, =_data_rom
	ldr	r1, =_data_start
	ldr	r2, =_data_end
0:
	cmp	r1, r2
	ldrlt	r3, [r0], #4
	strlt	r3, [r1], #4
	blt	0b

	/* Clear bss */
	ldr	r0, =_bss_start
	ldr	r1, =_bss_end
	mov	r2, #0
1:
	cmp	r0, r1
	strlt	r2, [r0], #4
	blt	1b

	/*
	 * Now need to setup the stack for every mode.
	 * cpsr_c only modifies control bits: mode and interrupts.
	 */
	msr	cpsr_c, #(FIQ_MASK | F_BIT | I_BIT)
	ldr	sp, =_fiq_stack_top
	msr	cpsr_c, #(IRQ_MASK | F_BIT | I_BIT)
	ldr	sp, =_irq_stack_top
	msr	cpsr_c, #(UND_MASK | F_BIT | I_BIT)
	ldr	sp, =_und_stack_top
	msr	cpsr_c, #(ABT_MASK | F_BIT | I_BIT)
	ldr	sp, =_abt_stack_top
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT)
	ldr	sp, =_kstack_top
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	ldr	sp, =_ustack_top

	bl	soc_init /* Does stack init, remapping, etc */
	bl	cpu_init
	bl	board_init /* Initialize board components */
	bl	os_init
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
