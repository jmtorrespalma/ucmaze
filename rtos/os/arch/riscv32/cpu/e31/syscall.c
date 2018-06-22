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
#include <ucmaze-os.h>
#include <syscall-sys.h>
#include <util/util.h>

/*
 * In riscv ABI we need to pass syscall number in a register since the ecall op
 * does not allow embedding numbers in the instruction.
 *
 * Thus, the kernel will expect the syscall number in a0, and following
 * arguments in following registers.
 */

void __attribute__((noreturn, naked)) _system_start(void)
{
	__asm__ volatile("li a0, " _xstr(SC_RESERVED) ";"
		"ecall"
		:::);
	__builtin_unreachable();
}

/*
 * Userspace wrapper, we need to pass parameters to the syscall handler in
 * registers.
 */
int task_create(int prio, void *entry, int argc, void *argv)
{
	int rv;

	__asm__ volatile("mv a4, %4;"
		"mv a3, %3;"
		"mv a2, %2;"
		"mv a1, %1;"
		"li a0, " _xstr(SC_TASK_CREATE) ";"
		"ecall;"
		"mv %0, a0"
		: "=r" (rv)
		: "r" (prio), "r" (entry), "r" (argc), "r" (argv)
		:);

	return rv;
}

void __attribute__((noreturn, naked)) task_exit(int exit_code)
{
	__asm__ volatile("mv a1, %0;"
		"li a0, " _xstr(SC_TASK_EXIT) ";"
		"ecall"
		:
		: "r" (exit_code)
		:);
	__builtin_unreachable();
}

void task_yield(void)
{
	__asm__ volatile("li a0, " _xstr(SC_TASK_YIELD) ";"
		"ecall"
		:::);
}

int irq_lock(void)
{
	int lock;

	__asm__ volatile("li a0, " _xstr(SC_IRQ_LOCK) ";"
		"ecall;"
		"mv %0, a0"
		: "=r" (lock)
		:
		:);

	return lock;
}

void irq_unlock(int lock)
{
	__asm__ volatile("mv a1, %0;"
		"li a0, " _xstr(SC_IRQ_UNLOCK) ";"
		"ecall"
		:
		: "r" (lock)
		:);
}
