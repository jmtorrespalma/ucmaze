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

#include <config.h>
#include <sched.h>
#include <irq.h>
#include <ucmaze-os.h>
#include <util/list.h>
#include <errno.h>

/*
 * Global array to keep kernel tasks, it provides the memory, while list
 * entries provide the logic.
 */
struct task task_mem[TASK_N_MAX];
extern struct list_head task_list; /* Sorted list */
extern struct task *task_current;

/*
 * Handle in kernel container of tasks.
 * Ideally, we would use kmalloc(), but no heap available.
 * Returns null if no spot available.
 */
struct task *task_alloc(void)
{
	struct task *it, *lmt;

	it = task_mem;
	lmt = task_mem + TASK_N_MAX;

	while (it != lmt) {
		if (it->state == TASK_FINISHED)
			return it;
		++it;
	}

	return NULL;
}

/*
 * Release the memory taken by the passed task.
 */
int task_free(uint8_t id)
{
	struct task *it, *lmt;

	it = task_mem;
	lmt = task_mem + TASK_N_MAX;

	while (it != lmt) {
		if (it->id == id) {
			it->state = TASK_FINISHED;
			return 0;
		}
		++it;
	}

	return -EINVAL;
}

/*
 * Generate a task id number.
 * We keep cached next value to assign.
 */
uint8_t tid_generate(void)
{
	static uint8_t next = TID_MIN;
	uint8_t to_ret;
	struct task *curr = NULL;

	to_ret = next;
	next++;

	/* Iterate over task_list */
	list_for_each_entry(curr, &task_list, le) {
		if (curr->id == next)
			next++;
	}

	return to_ret;
}

/*
 * Every task when running for the first time starts with this code. That is
 * done in a way that we can assure it will release resources after finishing
 * its function.
 *
 * Note that this code runs in userspace! Technically should be part of a C
 * library.
 */
void task_bootstrap(int argc, void *argv)
{
	int rv;

	rv = task_current->code(argc, argv);

	/* Syscall to release resources */
	task_exit(rv);
}

/*
 * Implementation of syscall to create a new task.
 */
int sys_task_create(int prio, void *entry, int argc, void *argv)
{
	struct task *new;
	uint8_t id;
	int key;

	key = sys_irq_lock();

	new = task_alloc();
	if (!new)
		return -ENOMEM;

	id = tid_generate();

	new->id = id;
	new->state = TASK_READY;
	new->prio = prio;
	new->code = entry;
	new->stack_ptr = new->stack_top; /* Set at os_boot() */

	context_init(new, argc, argv);
	sched_enqueue(new);

	sys_irq_unlock(key);

	return 0;
}

/*
 * Removes current thread and releases resources
 */
int sys_task_exit(int exit_code)
{
	int key;

	key = sys_irq_lock();

	sched_dequeue(task_current);
	task_free(task_current->id);

	task_current = sched_get_next();
	context_fake(task_current);
	sched_need_resched();

	sys_irq_unlock(key);

	return exit_code;
}
