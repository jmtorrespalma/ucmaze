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

#ifndef SYSCALL_SYS_H_
#define SYSCALL_SYS_H_

#include <ucmaze-os.h>

/*
 * Number zero is reserved for kernel internal implementation, using it will
 * cause nothing but trouble.
 */
#define SC_RESERVED     0
#define SC_TASK_CREATE  1
#define SC_TASK_EXIT    2
#define SC_TASK_YIELD   3
#define SC_IRQ_LOCK     4
#define SC_IRQ_UNLOCK   5
#define SYSCALL_MAX     6

/*
 * Implementation for every syscall.
 */
int sys_task_create(int prio, void *entry, int argc, void *argv);
void sys_task_exit(int exit_code);
void sys_task_yield(void);
int sys_irq_lock(void);
void sys_irq_unlock(int key);

#endif /* SYSCALL_SYS_H_ */
