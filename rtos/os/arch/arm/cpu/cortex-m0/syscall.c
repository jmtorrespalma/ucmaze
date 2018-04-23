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
#include <errno.h>
#include <ucmaze-os.h>
#include <syscall-sys.h>

#define SC_OS_INIT      0
#define SC_TASK_CREATE  1
#define SC_TASK_EXIT    2
#define SYSCALL_MAX     3

#define _str(_a) #_a
#define _xstr(_a) _str(_a)

/*
 * The syscall ABI expects parameters in r0-r7
 * The parameter collection must be done in asm, as well as the parameter
 * setup.
 */

static inline uint8_t _get_sc_num(void)
{
	uint8_t sc_num;

	__asm__ volatile("mrs r0, psp;"
		"ldr r1, [r0, #24];"
		"sub r1, r1, #2;"
		"ldrb %0, [r1]"
		: "=r" (sc_num)
		:
		: "r0", "r1", "memory");

	return sc_num;
}

/*
 * TODO: extend more parameters
 */
int _syscall_handler(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{

	uint8_t sc_num;
	int rv = -EINVAL;

	sc_num = _get_sc_num();

	if (sc_num >= SYSCALL_MAX)
		return rv;

	switch (sc_num) {
	case SC_OS_INIT:
		sys_os_init();
		break;
	case SC_TASK_CREATE:
		rv = sys_task_create((int)r0, (void *)r1, (int)r2, (void *)r3);
		break;
	case SC_TASK_EXIT:
		rv = sys_task_exit((int)r0);
		break;
	}

	return rv;
}

/*
 * Used because on cortex-m0 we start in non-privileged mode, so we need to
 * switch to privileged mode using svc and initialize all kernel data
 * structures.
 */
int os_init(void)
{
	int rv;

	//__asm__ volatile("svc #" _strf(SC_OS_INIT) ";"
	__asm__ volatile("svc #" _xstr(SC_OS_INIT) ";"
		"mov %0, r0"
		: "=r" (rv)
		:
		:);

	return rv;
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

int task_exit(int exit_code)
{
	int rv;

	__asm__ volatile("mov r0, %1;"
		"svc #" _xstr(SC_TASK_EXIT) ";"
		"mov %0, r0"
		: "=r" (rv)
		: "r" (exit_code)
		:);

	return rv;
}
