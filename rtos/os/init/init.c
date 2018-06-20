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

#include <irq.h>
#include <dev.h>
#include <ticker.h>
#include <sched.h>

/*
 * This needs to run in privileged mode.
 * Takes care of initializating all kernel data structures.
 * Add default tasks, select initial task and switch on the system
 * devices so we can start running userspace tasks.
 */
void os_init(void)
{
	/*
	 */
	sched_init();
	dev_init();
}

/*
 * Called from the reset handler.
 * On return it should typically start running the very first task and never be
 * called again unless very specific situations (deep sleep wakeup).
 */
void system_start(void)
{
	int key;
	key = k_lock();

	os_init();

	k_unlock(key);
}
