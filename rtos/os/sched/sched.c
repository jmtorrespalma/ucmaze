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
#include <util/mblock.h>

/*
 * Queue containing all system tasks
 */
LIST_HEAD(task_list);

/*
 * System runqueue, contains all runnable tasks for a given scheduling cycle.
 */
struct sched_rq sys_rq;

extern struct mblock task_mem;

/*
 * Initialize structures used by the scheduler.
 */
static void sched_mem_init(void)
{
	mblock_init(&task_mem);
	INIT_LIST_HEAD(&sys_rq.rq_head);
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
	struct task *first;

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
	sched_new_cycle(&sys_rq);
	first = sched_get_next(&sys_rq);
	sched_set_current(&sys_rq, first);
	context_fake(&first->stack);

	return 0;
}

/*
 *
 */
struct task *sched_get_next(struct sched_rq *rq)
{
	return list_first_entry(&rq->rq_head, struct task, entry);
}

struct task *sched_get_current(struct sched_rq *rq)
{
	return rq->curr;
}

void sched_set_current(struct sched_rq *rq, struct task *t)
{
	rq->curr = t;
}

/*
 * Here is implemented the logic of the scheduler to decide which task is next.
 * This function should be only called once per tick.
 */
void sched_update(struct sched_rq *rq)
{
	rq->curr->sched_status.remain_slices--;
}

int sched_need_resched(struct sched_rq *rq)
{
	return (rq->curr->sched_status.remain_slices == 0);
}

void sched_enqueue(struct sched_rq *rq, struct task *t)
{
	list_add_tail(&rq->rq_head, &t->sched_status.entry);
}

void sched_dequeue(struct task *t)
{
	list_del(&t->sched_status.entry);
}

int sched_cycle_over(struct sched_rq *rq)
{
	return list_empty(&rq->rq_head);
}

#define prio_to_slices(_prio) (((PRIO_MIN - (_prio)) >> 4) + 1)

/*
 * Iterate over task_list, getting all tasks that are ready and set their
 * sched_status field.
 * Only repopulates runqueue, current needs to be set by other
 * code.
 */
void sched_new_cycle(struct sched_rq *rq)
{
	struct task *it;
	struct sched_info *sit;

	list_for_each_entry(it, &task_list, entry) {
		sit = &it->sched_status;
		if (sit->state == TASK_READY) {
			sit->alloc_slices = prio_to_slices(sit->prio);
			sit->remain_slices = sit->alloc_slices;
			sched_enqueue(rq, it);
		}
	}
}
