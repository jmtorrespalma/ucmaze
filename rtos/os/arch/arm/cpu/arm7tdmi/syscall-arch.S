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

#define R0_OFF 8

	.syntax unified
	.code 32

	.section .text, "ax"
	.global syscall_retval
	.func   syscall_retval
/*
 * This is called by syscalls that need to return a value to the userspace,
 * so we need to write to the stacked r0, so the ABI will pickup the value as
 * expected.
 */
syscall_retval:
	msr	cpsr_c, #(SYS_MASK | F_BIT | I_BIT)
	str	r0, [sp, #R0_OFF]
	msr	cpsr_c, #(SVC_MASK | F_BIT | I_BIT)
	bx	lr

        .size   syscall_retval, . - syscall_retval
        .endfunc
