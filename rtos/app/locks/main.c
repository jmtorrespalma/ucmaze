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
#include <kstdio.h>

/*
 * Print with locked interrupts.
 */
int print_atomic(int argc, unsigned int *argv)
{
	char *string;
	int key;

	string = (char *)argv;

	while (1) {
		key = irq_lock();
		kputs(string);
		irq_unlock(key);
	}
}

char st1[] = "Task 1 printing";
char st2[] = "Task 2 printing";

int main(void)
{
	/* Create both threads */
	task_create(20, print_atomic, 1, st1);
	task_create(20, print_atomic, 1, st2);

	return 0;
}
