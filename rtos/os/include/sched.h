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

#ifndef SCHED_H_
#define SCHED_H_

#include <stdint.h>
#include <util/list.h>

enum task_state {
	TASK_FINISHED = 0,
	TASK_READY,
	TASK_BLOCKED
};

/*
 * Information used by the scheduler.
 */
struct sched_info {
	uint8_t prio;
	uint8_t alloc_slices;
	uint8_t remain_slices;
	enum task_state state;
	struct list_head sched_entry;
};

/*
 * Task stack userspace pointers. They are required for context switching.
 */
struct ustack {
	void *top;
	void *ptr;
};

/*
 * Task definition.
 */
struct task {
	uint8_t id;
	int (*code) (int, void *);
	struct ustack stack;
	struct sched_info sched_status;
	struct list_head task_entry; /* Used in task_list */
};

/*
 * Runqueue, contains system tasks.
 * THe actual stored item is a sched_info, cause it isolates all of the
 * information required for scheduling.
 */
struct sched_rq {
	struct task *curr;
	struct list_head rq_head;
};

/*
 * Scheduler calls.
 */
int sched_init(void);
void sched_update(struct sched_rq *rq);
void sched_enqueue(struct sched_rq *rq, struct task *t);
void sched_dequeue(struct task *t);

struct task *sched_get_next(struct sched_rq *rq);
struct task *sched_get_current(struct sched_rq *rq);
void sched_set_current(struct sched_rq *rq, struct task *t);

/*
 * Related to current scheduler.
 */
void sched_new_cycle(struct sched_rq *rq);
int sched_cycle_over(struct sched_rq *rq);
int sched_need_resched(struct sched_rq *rq);

/*
 * Wrapper around threads main function, used to setup the environment before,
 * and the cleanup after finishing.
 */
void task_start(int (*code)(int, void *), int argc, void *argv);

/*
 * Architecture dependent functions that setup data in a format expected by the
 * cpu so it's able to load tasks or remove them.
 */
void context_init(struct ustack *us, int (*code)(int, void *), int argc,
		  void *argv);
void context_switch(struct task *new, struct task *curr);
void context_fake(struct ustack *us);

#endif /* SCHED_H_ */
