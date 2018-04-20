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

#include <sched.h>

/*
 * Number of timer ticks since boot.
 */
unsigned int jiffies = 0;

extern struct task *task_current;

/*
 * High level ticker handler function.
 * Maybe better to only set a flag and switch somewhere else.
 * Check that no callee-saved register is pushed
 */
void ticker_handler(void)
{
	struct task *next;

	sched_need_resched();

	++jiffies;
}
