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

#define USTACK_OFF     8
#define STACK_PTR_OFF  4

	.syntax unified
	.code 32

	.section .text, "ax"
	.global context_switch
	.func   context_switch
/*
 * When we get to this point, we know for sure we need to swap 2 tasks that
 * have been passed as parameters. Actually, we only need to change the stack
 * pointer since all registers will be restored from there when exiting the
 * isr.
 *
 * Obviously this is running with interrupts locked.
 */

context_switch:
	mrs	r3, cpsr
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	str	sp, [r1, #(USTACK_OFF + STACK_PTR_OFF)]
	ldr	sp, [r0, #(USTACK_OFF + STACK_PTR_OFF)]
	msr	cpsr_c, r3
	bx	lr

        .size   context_switch, . - context_switch
        .endfunc

/*
 * Used when we want to remove the context of the calling task (i.e. upon OS
 * initialization or task exiting).
 *
 * Need to change to system mode and write the user stack to continue running
 * another task after the kernel returns.
 *
 * Passed parameter is a pointer to ustack
 */
	.global context_fake
	.func   context_fake
context_fake:
	mrs	r3, cpsr
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	ldr	sp, [r0, #(STACK_PTR_OFF)]
	msr	cpsr_c, r3
	bx	lr

        .size   context_fake, . - context_fake
        .endfunc
