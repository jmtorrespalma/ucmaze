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
#include "arm-reg.h"

/*
 * Only regular arm is supported, thumb tasks will cause strange behaviour.
 */
#define CPSR_INIT  (USR_MASK)

/*
 * Populates this task stack, so when the scheduler picks it, can start running
 * such as if it was resumed.
 */
void context_init(struct ustack *us, int (*code)(int, void *), int argc,
		  void *argv)
{
	uint32_t *stack_it;

	stack_it = us->top;

	/*
	 * Fill callee saved registers with garbage.
	 */
	*stack_it-- = 0; /* R11 */
	*stack_it-- = 0; /* R10 */
	*stack_it-- = 0; /* R9 */
	*stack_it-- = 0; /* R8 */
	*stack_it-- = 0; /* R7 */
	*stack_it-- = 0; /* R6 */
	*stack_it-- = 0; /* R5 */
	*stack_it = 0; /* R4 */

	*stack_it-- = 0; /* R3 */
	*stack_it-- = (uint32_t)argv; /* R2 */
	*stack_it-- = (uint32_t)argc; /* R1 */
	*stack_it-- = (uint32_t)code; /* R0 */

	*stack_it-- = 0; /* R12 */
	*stack_it-- = 0; /* LR */
	*stack_it-- = (uint32_t)task_start; /* PC */
	*stack_it-- = CPSR_INIT;

	us->ptr = stack_it;
}
