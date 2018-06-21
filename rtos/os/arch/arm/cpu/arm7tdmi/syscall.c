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
 * This is not a syscall, but a fixup due to ARM cores starting in
 * non-privileged mode.
 */
void __attribute__((noreturn, naked)) _system_start(void)
{
	__asm__ volatile("svc #" _xstr(SC_RESERVED) ";" :::);
	__builtin_unreachable();
}

/*
 * Userspace wrapper, we need to pass parameters to the syscall handler in
 * registers.
 */
int task_create(int prio, void *entry, int argc, void *argv)
{
	int rv;

	__asm__ volatile("mov r0, %1;"
		"mov r1, %2;"
		"mov r2, %3;"
		"mov r3, %4;"
		"svc #" _xstr(SC_TASK_CREATE) ";"
		"mov %0, r0"
		: "=r" (rv)
		: "r" (prio), "r" (entry), "r" (argc), "r" (argv)
		:);

	return rv;
}

void __attribute__((noreturn, naked)) task_exit(int exit_code)
{
	__asm__ volatile("svc #" _xstr(SC_TASK_EXIT) ";":::);
	__builtin_unreachable();
}

void task_yield(void)
{
	__asm__ volatile("svc #" _xstr(SC_TASK_YIELD) ";":::);
}

int irq_lock(void)
{
	int lock;

	__asm__ volatile("svc #" _xstr(SC_IRQ_LOCK) ";"
		"mov %0, r0"
		: "=r" (lock)
		:
		:);

	return lock;
}

void irq_unlock(int lock)
{
	__asm__ volatile("mov r0, %0;"
		"svc #" _xstr(SC_IRQ_UNLOCK)
		:
		: "r" (lock)
		:);
}
