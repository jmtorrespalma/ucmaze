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

#ifndef UCMAZE_OS_H_
#define UCMAZE_OS_H_

/*
 * Public API, only these functions should be exposed to the application.
 */
int task_create(int prio, void *entry, int argc, void *argv);
void task_exit(int exit_code);
void task_yield(void);
int irq_lock(void);
void irq_unlock(int key);

#endif /* UCMAZE_OS_H_ */
