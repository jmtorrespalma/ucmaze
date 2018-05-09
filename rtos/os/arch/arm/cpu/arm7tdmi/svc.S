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
 * Low level handler for syscalls, we need to link this with the kernel syscall
 * interface. In this architecture system calls are non-interruptible.
 */

#include "arm-reg.h"

	.syntax unified
	.code 32

/*
 * Need to save context because some syscalls will call context_switch() or
 * leave the current context, like task_exit().
 */
	.section .text, "ax"
	.global _svc_handler
	.func   _svc_handler
_svc_handler:
	/*
	 * Save context in user stack.
	 */
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	push	{r0-r10,fp,ip,lr} /* Save r0-r12, lr */
	mov	r4, sp
	sub	sp, sp, #8
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT)
	mrs	r5, spsr
	stmdb	r4!, {r5, lr} /* Save thread status and pc */

	/*
	 * Pass parameters.
	 */
	push	{r3, lr} /* To keep 8 bit aligned */
	mov	r3, r2
	mov	r2, r1
	mov	r1, r0
	ldrb	r0, [lr, #-4] /* lr does not need adjustment */

	/* Call kernel generic syscall */
	bl	syscall_handler
	pop	{r7, lr}

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

        .size   _svc_handler, . - _svc_handler
        .endfunc
