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

	.section .text, "ax"
	.global syscall_retval
	.func   syscall_retval
	.thumb_func
/*
 * psp is pointing to the stacked r0, so we just need to write there.
 */
syscall_retval:
	mrs	r1, psp
	str	r0, [r1]
	bx	lr

        .size   syscall_retval, . - syscall_retval
        .endfunc
