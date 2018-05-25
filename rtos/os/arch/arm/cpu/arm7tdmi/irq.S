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
	sub	lr, lr, #4 /* Adjust interrupted pc */
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	push	{r0-r10,fp,ip,lr} /* Save r0-r12, lr */
	mov	r0, sp
	sub	sp, sp, #8
	msr	cpsr_c, #(IRQ_MASK | F_BIT | I_BIT)
	mrs	r1, spsr
	stmdb	r0!, {r1, lr} /* Save thread status and pc */

	/* Call soc handler */
	bl	_soc_irq_handler

	/* Restore context */
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	ldr	lr, [sp, #60] /* Loads usermode lr */
	mov	r0, sp
	add	sp, sp, #64
	msr	cpsr_c, #(IRQ_MASK | F_BIT | I_BIT)
	ldr	lr, [r0], #4
	msr	spsr, lr
	ldr	lr, [r0], #4 /* Return address */
	ldmia	r0, {r0-r10,fp,ip}

	/*
	 * Return to userspace.
	 */
	 push	{lr}
	 ldmia	sp!, {pc}^

        .size   _irq_handler, . - _irq_handler
        .endfunc

/*
 * Routines for interrupt lock and unlock.
 * The kernel always runs locked for this cpu, so
 * k_lock() and k_unlock() do nothing.
 */
	.global k_lock
	.func   k_lock
k_lock:
	mov	r0, #0
	bx	lr

        .size   k_lock, . - k_lock
        .endfunc

	.global k_unlock
	.func   k_unlock
k_unlock:
	bx	lr

        .size   k_unlock, . - k_unlock
        .endfunc

/*
 * Syscall implementation to modify lock or unlock interrupts.
 */
	.global sys_irq_lock
	.func   sys_irq_lock
sys_irq_lock:
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	ldr	r0, [sp]
	orr	r1, r0, #(F_BIT | I_BIT)
	str	r1, [sp]
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT)
	bx	lr

        .size   sys_irq_lock, . - sys_irq_lock
        .endfunc

	.global sys_irq_unlock
	.func   sys_irq_unlock
sys_irq_unlock:
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	str	r0, [sp]
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT)
	bx	lr

        .size   sys_irq_unlock, . - sys_irq_unlock
        .endfunc
