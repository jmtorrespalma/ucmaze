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
 * The low-level syscall handler takes care of:
 * -Extracting the svc number and dentify the high level syscall.
 * -Format parameters and call it.
 * -Rearrange return values.
 */

#define SC_TASK_EXIT 2

	.syntax unified
	.thumb

	.section .text, "ax"
	.global _syscall_handler
	.func   _syscall_handler
	.thumb_func
_syscall_handler:
	/*
	 * Lock interrupts and try to use as few registers
	 * svc number.
	 */
	cpsid	i
	sub	sp, sp, #12       /* Temporary variables */
	str	r3, [sp]
	str	r4, [sp, #4]
	mov	r3, lr
	str	r3, [sp, #8]

	mov	r3, r0
	mov	r4, r1

	mrs	r1, psp
	ldr	r0, [r1, #24]
	subs	r1, r0, #2       /* pc correction */
	ldrb	r0, [r1]

	/*
	 * Setup parameters
	 */
	mov	r1, r3 /* Old r0 */
	mov	r3, r2
	mov	r2, r4 /* Old r1 */

	cmp	r0, #SC_TASK_EXIT
	beq	0f

	/*
	 * Regular syscall
	 */
	cpsie	i
	blx	syscall_handler
	ldr	r1, [sp, #8]
	mov	lr, r1
	ldr	r4, [sp, #4]
	add	sp, sp, #12
	bx	lr

0:
	/*
	 * task_exit() handler, runs locked all the way and needs exit glue
	 * code to restore newly selected task context.
	 */
	mov	r4, lr
	blx	sys_task_exit
	mov	lr, r4

	mrs	r0, psp
	subs	r0, r0, #16
	stmia	r0!, {r4-r7}
	mov	r8, r4
	mov	r9, r5
	mov	r10, r6
	mov	r11, r7
	subs	r0, r0, #32
	stmia	r0!, {r4-r7}
	add	sp, sp, #12
	cpsie	i
	bx	lr

        .size   _syscall_handler, . - _syscall_handler
        .endfunc
