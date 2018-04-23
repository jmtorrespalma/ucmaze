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

/*
 * System call to temporarily disable interrupts.
 * In cortex-m0, we actually disable all exceptions, but those unmaskable.
 * If previously disabled, will return value different to zero.
 */
unsigned int sys_irq_lock(void)
{
	unsigned int key;

	__asm__ volatile("mrs %0, PRIMASK;"
		"cpsid i"
		: "=r" (key)
		:
		: "memory");

	return key;
}

/*
 * Restaurates previous interrupt status.
 * If key is different to zero, exceptions were off before us.
 */
void sys_irq_unlock(unsigned int key)
{

	if (key)
		return;

	__asm__ volatile("cpsie i" : : : "memory");
}
