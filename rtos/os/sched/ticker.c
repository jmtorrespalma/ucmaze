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
#include <irq.h>

/*
 * System runqueue
 */
extern struct sched_rq sys_rq;

/*
 * Number of timer ticks since boot.
 */
unsigned int jiffies = 0;

/*
 * High level ticker handler function.
 * Typically called from a lower level ISR.
 * XXX: remove locks?
 */
void ticker_handler(void)
{
	int key;

	key = k_lock();

	sched_update(&sys_rq);

	if (sched_need_resched(&sys_rq))
		sched_schedule(&sys_rq);

	++jiffies;
	k_unlock(key);
}
