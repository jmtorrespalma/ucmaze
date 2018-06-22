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
#include "riscv.h"

/*
 * Initial mstatus value for a task.
 * Need to take care that some values will be set after returning to userspace,
 * so instead of setting mie, need to set mpie so the hardware will restore it
 * automatically.
 *
 * The task will start running with machine interrupts on and in machine mode,
 * the only one supported so far.
 */
#define MSTATUS_INIT  (MSTATUS_MPIE | MSTATUS_MPP)

/*
 * Writes the stack register where the kernel is planning to run next.
 */
void context_fake(struct ustack *us)
{
	uintptr_t *sp = us->ptr;

	write_csr(mscratch, sp);
}

/*
 * Populates a stack passed in us. The kernel ABI to restore and save tasks is
 * specified here. This needs to be aligned with _trap_entry() since there we
 * also restore and save context.
 *
 * Some registers like gp or tp stay the same during the whole execution,
 * so makes no * sense to save and restore them.
 */
void context_init(struct ustack *us, int (*code)(int, void *), int argc,
		  void *argv)
{
	uintptr_t *stack_it;

	stack_it = us->top;

	/*
	 * We don't save stack pointer in the stack, it's saved on the ustack
	 * passed.
	 * The saved context size is 120B for RV32.
	 */
	*stack_it-- = 0; /* x1/ra: return address */
	*stack_it-- = 0; /* x5/t0: temporary */
	*stack_it-- = 0; /* x6/t1: temporary */
	*stack_it-- = 0; /* x7/t2: temporary */
	*stack_it-- = 0; /* x8/s0/fp: frame pointer */
	*stack_it-- = 0; /* x9/s1: saved register */
	*stack_it-- = (uintptr_t)code; /* x10/a0: argument */
	*stack_it-- = (uintptr_t)argc; /* x11/a1: argument */
	*stack_it-- = (uintptr_t)argv; /* x12/a2: argument */
	*stack_it-- = 0; /* x13/a3: argument */
	*stack_it-- = 0; /* x14/a4: argument */
	*stack_it-- = 0; /* x15/a5: argument */
	*stack_it-- = 0; /* x16/a6: argument */
	*stack_it-- = 0; /* x17/a7: argument */
	*stack_it-- = 0; /* x18/s2: saved */
	*stack_it-- = 0; /* x19/s3: saved */
	*stack_it-- = 0; /* x20/s4: saved */
	*stack_it-- = 0; /* x21/s5: saved */
	*stack_it-- = 0; /* x22/s6: saved */
	*stack_it-- = 0; /* x23/s7: saved */
	*stack_it-- = 0; /* x24/s8: saved */
	*stack_it-- = 0; /* x25/s9: saved */
	*stack_it-- = 0; /* x26/s10: saved */
	*stack_it-- = 0; /* x27/s11: saved */
	*stack_it-- = 0; /* x28/t3: temporary */
	*stack_it-- = 0; /* x29/t4: temporary */
	*stack_it-- = 0; /* x30/t5: temporary */
	*stack_it-- = 0; /* x31/t6: temporary */

	*stack_it-- = (uintptr_t)task_start; /* code to run */
	*stack_it = MSTATUS_INIT;

	us->ptr = stack_it;
}

void context_switch(struct task *new, struct task *curr)
{
	uintptr_t *curr_sp, *new_sp;

	new_sp = new->stack.ptr;

	__asm__ volatile("csrrw %0, mscratch, %1"
		: "=r" (curr_sp)
		: "r" (new_sp)
		: "memory");

	curr->stack.ptr = curr_sp;
}
