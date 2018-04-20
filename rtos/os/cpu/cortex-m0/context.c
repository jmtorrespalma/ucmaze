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

#define XPSR_INIT  (0x01000000) /* Avoid clearing T bit */
#define EXC_RETURN (0xfffffffd)

/*
 * Context switch is highly ABI and architecture dependent, so we stick to EABI
 * and regular exception model from cortex m0.
 */

/*
 * Assumes that xPSR, PC, LR, R12, R3-R0 are already stacked in PSP
 * and kernel kept r4-r11 unmodified
 */

/*
 * Same as context_save(), but saves extra registers that usually are preserved
 * by the processor.
 * Also, lr must be set manually to EXC_RETURN.
 * Probably this function needs to be *always* inline, or totally in assembly.
 *
 * This is called the first scheduling ever only, so the kernel when returning
 * to userspace has the expected context to restore the first task.
 *
 * Previous value of psp is lost, but that's ok since it was used for
 * initialization which is about to finish.
 */
void context_fake(struct task *t)
{
	uint32_t *fake_psp;

	fake_psp = t->stack_ptr;
	fake_psp += 8; /* Offset to CPU saved registers */

	__asm__ volatile("msr psp, %0"
		:
		: "r" (fake_psp)
		: "memory");
}

/*
 * Populates this task stack, so when the scheduler picks it, can start running
 * such as if it was resumed.
 * r0: num of args
 * r1: pointer to args
 * pc: task_bootstrap()
 * psp: stack_top + sizeof(context)
 *
 * update stack variable.
 */
void context_init(struct task *t, int argc, void *argv)
{
	uint32_t *stack_it;

	stack_it = t->stack_top;

	*stack_it-- = XPSR_INIT;
	*stack_it-- = (uint32_t)task_bootstrap; /* PC */
	*stack_it-- = 0; /* LR */
	*stack_it-- = 0; /* R12 */
	*stack_it-- = 0; /* R3 */
	*stack_it-- = 0; /* R2 */
	*stack_it-- = (uint32_t)argv; /* R1 */
	*stack_it-- = (uint32_t)argc; /* R0 */

	/* Now fill with garbage the rest of registers */
	*stack_it-- = 0; /* R11 */
	*stack_it-- = 0; /* R10 */
	*stack_it-- = 0; /* R9 */
	*stack_it-- = 0; /* R8 */
	*stack_it-- = 0; /* R7 */
	*stack_it-- = 0; /* R6 */
	*stack_it-- = 0; /* R5 */
	*stack_it = 0; /* R4 */

	t->stack_ptr = stack_it;
}

void sched_need_resched(void)
{
	volatile uint32_t *icsr = (uint32_t *)0xe000ed04;
	*icsr |= (1 << 28); /* Sets pendsv signal */
}
