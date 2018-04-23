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

#include <stdlib.h>
#include <config.h>
#include <sched.h>
#include <syscall-sys.h>
#include <util/list.h>

/*
 * Queue containing all system tasks
 */
LIST_HEAD(task_list);

extern char _ustack_top;
extern struct task task_mem[TASK_N_MAX];

/*
 * Current task set to run.
 */
struct task *task_current;

/*
 * Initialize structures used by the scheduler.
 */
void sched_mem_init(void)
{
	char *ustack_ptr = &_ustack_top; /* Accessing linker symbol */

	for (int i = 0; i < TASK_N_MAX; ++i) {
		task_mem[i].state = TASK_FINISHED;
		task_mem[i].stack_top = ustack_ptr - STACK_SZ * i;
	}
}

/*
 * Forward declaration of main, this is necessary for the first task on the os
 * to run it, either if it's multitask or single task application.
 */
extern int main(void);

/*
 * Initialize internal data required for scheduling
 */
int sched_init(void)
{
	int rv;

	sched_mem_init();

	/* If single task, must run the main function */
	rv = sys_task_create(200, main, 0, NULL);
	if (rv)
		return rv;

	#ifdef CONFIG_IDLE_TASK
	rv = sys_task_create(PRIO_MIN, idle, 0, NULL);
	if (rv)
		return rv;
	#endif /* CONFIG_IDLE_TASK */

	/*
	 * Here we pick the first task and fake it as if it was running before
	 * so when we return to usermode, this task will start running.
	 */
	task_current = sched_get_next();
	context_fake(task_current);

	return 0;
}

/*
 * Currently just picks the thread ready with highest priority
 */
struct task *sched_get_next(void)
{
	struct task *it, *next = NULL;;
	static int order = 0;

	if (order)
		list_for_each_entry(it, &task_list, le) {
			if (it->state == TASK_READY) {
				if (!next)
					next = it;
				else if (it->prio < next->prio)
					next = it;
			}
		}
	else
		list_for_each_entry_reverse(it, &task_list, le) {
			if (it->state == TASK_READY) {
				if (!next)
					next = it;
				else if (it->prio < next->prio)
					next = it;
			}
		}


	order = !order;

	return next;
}

/*
 * Add new task to execution list.
 */
void sched_enqueue(struct task *t)
{
	list_add_tail(&t->le, &task_list);
}

/*
 * Obviously will crash if task is not in the list.
 */
void sched_dequeue(struct task *t)
{
	list_del(&t->le);
}
