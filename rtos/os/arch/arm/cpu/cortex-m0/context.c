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

/*
 * Context switch is highly ABI and architecture dependent, so we stick to EABI
 * and regular exception model from cortex m0.
 *
 * Cortex-m0 context assumes that xPSR, PC, LR, R12, R3-R0 are already stacked
 * in PSP and kernel kept r4-r11 unmodified.
 */

#define XPSR_INIT  (0x01000000) /* Avoid clearing T bit */
#define EXC_RETURN (0xfffffffd)

/*
 * These two are necessary because within context_switch() and
 * _context_switch() the real context is not sys_rq->curr, due to real context
 * switch taking place in PendSV.
 */
volatile struct ustack *_curr_context;

/*
 * Used when we want to remove the context of the calling task (i.e. upon OS
 * initialization or task exiting).
 *
 * Previous value of psp is lost, so need to make sure that stack is previously
 * wiped out or overwritten.
 */
void context_fake(struct ustack *us)
{
	uint32_t *fake_psp;

	fake_psp = us->ptr;
	fake_psp += 8; /* Offset to CPU saved registers */

	__asm__ volatile("msr psp, %0"
		:
		: "r" (fake_psp)
		: "memory");
}

/*
 * Populates this task stack, so when the scheduler picks it, can start running
 * such as if it was resumed.
 */
void context_init(struct ustack *us, int (*code)(int, void *), int argc,
		  void *argv)
{
	uint32_t *stack_it;

	stack_it = us->top;

	*stack_it-- = XPSR_INIT;
	*stack_it-- = (uint32_t)task_start; /* PC */
	*stack_it-- = 0; /* LR */
	*stack_it-- = 0; /* R12 */
	*stack_it-- = 0; /* R3 */
	*stack_it-- = (uint32_t)argv; /* R2 */
	*stack_it-- = (uint32_t)argc; /* R1 */
	*stack_it-- = (uint32_t)code; /* R0 */

	/* Now fill with garbage the rest of registers */
	*stack_it-- = 0; /* R11 */
	*stack_it-- = 0; /* R10 */
	*stack_it-- = 0; /* R9 */
	*stack_it-- = 0; /* R8 */
	*stack_it-- = 0; /* R7 */
	*stack_it-- = 0; /* R6 */
	*stack_it-- = 0; /* R5 */
	*stack_it = 0; /* R4 */

	us->ptr = stack_it;
}

/*
 * Implementation for cortex-m0 simply sets the flags for PendSV exception, and
 * it takes care of doing the switch with lower priority.
 */
#define PENDSV_BIT (1 << 28)
void context_switch(struct task *new, struct task *curr)
{
	volatile uint32_t *icsr = (uint32_t *)0xe000ed04;

	/*
	 * context_switch() has been called again before a previous context
	 * switch request has been completed, probably due to a higher priority
	 * interrupt. It means that curr has not run at all but its slices have
	 * been consumed.
	 */
	if (*icsr & PENDSV_BIT) {
		/* nop */
	} else {
		*icsr |= PENDSV_BIT; /* Sets pendsv signal */
		_curr_context = &curr->stack;
	}
}
