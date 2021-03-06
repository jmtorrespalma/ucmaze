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
#include <irq.h>
#include <sched.h>
#include <syscall-sys.h>
#include <ucmaze-os.h>
#include <util/list.h>
#include <util/mblock.h>
#include <errno.h>

/*
 * Global array to keep kernel tasks, it provides the memory, while list
 * entries provide the logic.
 */
extern struct list_head task_list; /* Sorted list */

/*
 * System runqueue.
 */
extern struct sched_rq sys_rq;

/*
 * Memory block to allocate new tasks.
 */
MBLOCK_DECLARE(task_mem, struct task, CONFIG_TASK_N_MAX);

/*
 * Handle in kernel container of tasks.
 * Ideally, we would use kmalloc(), but no heap available.
 * Returns null if no spot available.
 */
struct task *task_alloc(void)
{
	return mblock_alloc(&task_mem);
}

/*
 * Release the memory taken by the passed task.
 */
void task_free(struct task *t)
{
	return mblock_free(&task_mem, t);
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
	list_for_each_entry(curr, &task_list, task_entry) {
		if (curr->id == next)
			next++;
	}

	return to_ret;
}

/*
 * Add new task to task_list
 */
void task_link(struct task *t)
{
	list_add_tail(&t->task_entry, &task_list);
}

/*
 * Obviously will crash if task is not in the list.
 */
void task_unlink(struct task *t)
{
	list_del(&t->task_entry);
}

/*
 * Initialized to zero.
 */
uint8_t ustack_tracker[CONFIG_TASK_N_MAX];
extern char _ustack_top;

void ustack_assign(struct ustack *us)
{
	char *ustack_ptr = &_ustack_top; /* Accessing linker symbol */

	for (int i = 0; i < CONFIG_TASK_N_MAX; ++i)
		if (ustack_tracker[i] == 0) {
			us->top = ustack_ptr - CONFIG_STACK_SZ * i;
			ustack_tracker[i] = 1;
			break;
		}
}

void ustack_release(struct ustack *us)
{
	char *ustack_ptr = &_ustack_top;
	int i = (int)(ustack_ptr - (char*)us->top) / CONFIG_STACK_SZ;

	ustack_tracker[i] = 0;
}

/*
 * Implementation of syscall to create a new task.
 */
int sys_task_create(int prio, void *entry, int argc, void *argv)
{
	struct task *new;
	uint8_t id;
	int key;

	key = k_lock();

	new = task_alloc();
	if (!new)
		return -ENOMEM;

	id = tid_generate();

	/*
	 * TODO: different function.
	 */
	new->sched_status.prio = prio;
	new->sched_status.state = TASK_READY;

	new->id = id;
	new->code = entry;

	/*
	 * Stack initialization.
	 */
	ustack_assign(&new->stack);
	context_init(&new->stack, entry, argc, argv);

	task_link(new); /* Adds it to task_list */

	k_unlock(key);

	return 0;
}

/*
 * Removes current thread and releases resources.
 * Called from a low level handler.
 */
void sys_task_exit(int exit_code)
{
	int key;
	struct task *curr, *next;

	key = k_lock();

	/*
	 * TODO: write the exit_code to a joined thread.
	 */
	(void)exit_code;

	/* Remove all links of the task in the kernel */
	curr = sched_get_current(&sys_rq);
	sched_dequeue(curr);
	task_unlink(curr);
	ustack_release(&curr->stack);
	task_free(curr);

	/* Update the runqueue */
	if (sched_cycle_over(&sys_rq))
		sched_new_cycle(&sys_rq);

	next = sched_get_next(&sys_rq);
	sched_set_current(&sys_rq, next);
	context_fake(&next->stack); /* Low level magic */

	k_unlock(key);
}

/*
 * Calls the scheduler to allow another threads to run instead of the current
 * one.
 */
void sys_task_yield(void)
{
	int key;

	key = k_lock();

	sched_schedule(&sys_rq);

	k_unlock(key);
}
