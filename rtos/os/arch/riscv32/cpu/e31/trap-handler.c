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
#include <syscall-sys.h>
#include "riscv.h"

/*
 * Forward declaration of kernel calls.
 */
extern void syscall_handler(uint32_t sc, uint32_t p0, uint32_t p1,
			    uint32_t p2, uint32_t p3);
extern void ticker_handler(void);
extern void ticker_reset(void);
extern void _syscall_get_params(uint32_t *sc, uint32_t *p0, uint32_t *p1,
				uint32_t *p2, uint32_t *p3);
extern void system_start(void);

/*
 * Use this handler for non-implemented or covered events, interrupts, etc.
 */
void error_handler(void)
{
	while (1);
}

void interrupt_handler(uintptr_t int_code)
{
	switch (int_code) {
	case MCAUSE_IC_MTI:
		ticker_handler();
		ticker_reset();
		break;
	default:
		error_handler();
	}
}

/*
 * Maps riscv synchronous traps to kernel handlers.
 */
void exception_handler(uintptr_t exc_code)
{
	uint32_t sc, p1, p2, p3, p4;

	if (exc_code != MCAUSE_EC_MEC)
		error_handler();

	/*
	 * Call generic kernel syscall handler.
	 */
	_syscall_get_params(&sc, &p1, &p2, &p3, &p4);
	if (sc == SC_RESERVED)
		system_start();
	else
		syscall_handler(sc, p1, p2, p3, p4);
}

/*
 * Transforms riscv traps to kernel format interrupt and system calls.
 *
 * ISRs have no parameters, so they can be called directly from here, but
 * exceptions or syscalls require user passed parameters.
 */
int trap_handler(uintptr_t mcause)
{
	uintptr_t exc_number;

	exc_number = (mcause & ~MCAUSE_INT);

	if (mcause & MCAUSE_INT)
		interrupt_handler(exc_number);
	else
		exception_handler(exc_number);

	return 0;
}
