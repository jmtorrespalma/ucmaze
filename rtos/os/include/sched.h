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
	TASK_RUNNING,
	TASK_READY,
	TASK_BLOCKED
};

struct task {
	uint8_t id;
	enum task_state state;
	uint8_t prio;
	int (*code) (int, void *);
	void *stack_top;
	void *stack_ptr; /* Used to restore context */
	struct list_head le;
};

int sched_init(void);
struct task *sched_get_next(void);
void sched_need_resched(void);


void task_bootstrap(int argc, void *argv);

void context_init(struct task *t, int argc, void *argv);
void context_fake(struct task *t);

#endif /* SCHED_H_ */
