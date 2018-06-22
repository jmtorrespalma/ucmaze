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

#include <stdint.h>
#include "riscv.h"

/*
 * Kernel hook to set a syscall return value in the task saved context.
 */
void syscall_retval(int rv)
{
	uintptr_t *sp;

	__asm__ volatile("csrr %0, mscratch"
		: "=r" (sp)
		:
		: "memory");

	sp[22] = rv; /* Writes to saved a0 */
}

/*
 * Inspects the calling task stack to retrieve its calling parameters.
 */
void _syscall_get_params(uint32_t *sc, uint32_t *p0, uint32_t *p1,
			 uint32_t *p2, uint32_t *p3)
{
	uintptr_t *sp;

	__asm__ volatile("csrr %0, mscratch"
		: "=r" (sp)
		:
		: "memory");

	*sc = sp[31];
	*p0 = sp[30];
	*p1 = sp[29];
	*p2 = sp[28];
	*p3 = sp[27];
}
