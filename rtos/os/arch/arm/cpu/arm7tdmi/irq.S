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
 * CPU level irq handler. Here we only save the contest, adjust the stack and
 * call _soc_irq_handler() that knows how to extract the source and find the
 * appropriate handler.
 */

#include "arm-reg.h"

	.syntax unified
	.code 32

/*
 * We do not support nested interrupts because of how the cpu works, being
 * difficult to handle tasks and nested interrupts at the same time.
 *
 * An irq can only happen from user mode, so we save the context on the task
 * that called us.
 */
	.section .text, "ax"
	.global _irq_handler
	.func   _irq_handler
_irq_handler:
	/*
	 * We save all context because an interrupt can endup with a
	 * context_switch() call, so we need to save the whole context.
	 * happened.
	 */
	push	{r0-r3}
	sub	r2, lr, #4 /* Adjust interrupted pc */
	mrs	r1, spsr

	/* Case if coming from userspace */
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	mov	r3, lr /* Userspace lr */
	mov	r0, sp
	sub	sp, sp, #64
	msr	cpsr_c, #(IRQ_MASK | F_BIT | I_BIT)
	stmdb	r0!, {r4-r11}
	pop	{r4-r7}
	stmdb	r0!, {r4-r7} /* Actually r0-r3 */
	stmdb	r0!, {r1-r3, r12} /* cspr, pc, lr, r12 */

	/* Call soc handler */
	bl	_soc_irq_handler

	/* Restore context */
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	mov	r0, sp
	add	sp, sp, #64
	msr	cpsr_c, #(IRQ_MASK | F_BIT | I_BIT)
	ldmia	r0!, {r1-r3, r12}
	msr	spsr, r1
	mov	lr, r2
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	mov	lr, r3
	msr	cpsr_c, #(IRQ_MASK | F_BIT | I_BIT)
	ldmia	r0, {r0-r11}

	/*
	 * Return to userspace.
	 */
	 push	{lr}
	 ldmia	sp!, {pc}^

        .size   _irq_handler, . - _irq_handler
        .endfunc

/*
 * Routines for interrupt lock and unlock
 */
	.global sys_irq_lock
	.func   sys_irq_lock
sys_irq_lock:
	mrs	r0, cpsr
	bic	r1, r0, #(F_BIT | I_BIT)
	msr	cpsr_c, r1
	bx	lr

        .size   sys_irq_lock, . - sys_irq_lock
        .endfunc

	.global sys_irq_unlock
	.func   sys_irq_unlock
sys_irq_unlock:
	msr	cpsr_c, r0
	bx	lr

        .size   sys_irq_unlock, . - sys_irq_unlock
        .endfunc