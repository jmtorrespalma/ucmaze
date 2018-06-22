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

extern void _trap_handler(void);

/*
 * Need to make sure that this value is 30 bits representable, otherwise it
 * will be set wrong.
 */
static int mvec_set(void (*func)(void))
{
	uintptr_t ufunc;

	ufunc = (uintptr_t)func;

	if (ufunc >= (1 << (XLEN - 2)))
		return -1;

	ufunc = ufunc << 2;

	__asm__ volatile("csrw mtvec, %0"
		:
		: "r" (ufunc)
		: "memory");

	return 0;
}

/*
 * Write mtvec to handle traps and interrupts.
 */
void cpu_init(void)
{
	mvec_set(_trap_handler);

}
