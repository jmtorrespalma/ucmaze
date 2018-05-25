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

#include <syscall-sys.h>
#include <errno.h>
#include <stdint.h>

/*
 * We need to somehow pass the return value to userspace, so syscall_retval()
 * writes that value wherever the application will be expecting it.
 */
extern void syscall_retval(int rv);

/*
 * High level syscall handler, we support up to 4 parameters.
 */
void syscall_handler(uint32_t sc_num, uint32_t p1, uint32_t p2,
		     uint32_t p3, uint32_t p4)
{
	int rv = 0;

	switch (sc_num) {
	case SC_TASK_CREATE:
		rv = sys_task_create((int)p1, (void *)p2, (int)p3, (void *)p4);
		syscall_retval(rv);
		break;
	case SC_TASK_EXIT:
		sys_task_exit((int)p1);
		break;
	case SC_TASK_YIELD:
		sys_task_yield();
		break;
	case SC_IRQ_LOCK:
		rv = sys_irq_lock();
		syscall_retval(rv);
		break;
	case SC_IRQ_UNLOCK:
		sys_irq_unlock((int)p1);
		break;
	default:
		while (1); /* TODO: Assert somehow */
		break;
	}
}
