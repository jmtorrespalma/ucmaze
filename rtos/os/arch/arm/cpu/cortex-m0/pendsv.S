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

#define USTACK_OFF     8
#define CURR_OFF       0
#define STACK_PTR_OFF  4

	.syntax unified
	.thumb

	.section .text, "ax"
	.global _context_switch
	.func   _context_switch
	.thumb_func
_context_switch:
	cpsid	i
	mrs	r0, psp
	subs	r0, r0, #32
	mov	r1, r0
	stmia	r0!, {r4-r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, r10
	mov	r7, r11
	stmia	r0!, {r4-r7}

	/* Update _curr_context->ptr */
	ldr	r0, =_curr_context
	ldr	r2, [r0]
	str	r1, [r2, #STACK_PTR_OFF]

	ldr	r0, =sys_rq
	ldr	r2, [r0, #CURR_OFF]  /* Get sys_rq->curr */
	ldr	r1, [r1, #(USTACK_OFF + STACK_PTR_OFF)]

	/* r1 contains sys_rq->curr->ustack.ptr */
	adds	r1, r1, #16
	ldmia	r1!, {r4-r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, r10
	mov	r7, r11
	subs	r1, r1, #32
	ldmia	r1!, {r4-r7}
	adds	r1, r1, #16
	msr	psp, r1

	cpsie	i
	bx	lr

        .size   _context_switch, . - _context_switch
        .endfunc
