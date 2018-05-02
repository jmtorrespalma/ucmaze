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
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT) /* Lock interrupts */
	push	{r0-r3}

	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	mov	r3, lr /* Userspace lr */
	mov	r0, sp
	sub	sp, sp, #64
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT)
	stmdb	r0!, {r4-r11}
	pop	{r4-r7}
	stmdb	r0!, {r4-r7} /* Original r0-r3 */
	stmdb	r0!, {r1-r3, r12} /* cspr, pc, lr, r12 */

	/*
	 * Pass parameters.
	 */
	push	{r7, lr} /* To keep 8 bit aligned */
	ldrb	r0, [lr, #-4] /* lr does not need adjustment */
	mov	r1, r4
	mov	r2, r5
	mov	r3, r6

	/* Call kernel generic syscall */
	bl	syscall_handler
	pop	{r7, lr}

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

        .size   _svc_handler, . - _svc_handler
        .endfunc
