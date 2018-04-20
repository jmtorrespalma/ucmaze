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

#include "cm-reg.h"

/*
 * OS stub necessary to turn on interrupts and another cpu dependent code.
 * In cortex-m0, we take care of:
 * - Set default exception priorities.
 * - Initialize NVIC, but do not start it yet.
 * - Set up ticker, but dont start it.
 * - Enable interrupts.
 */
void cpu_init(void)
{
	/* Systick gets high priority */
	SCB->shpr3 |= (0x0 << 8);

	/* Syscall medium */
	SCB->shpr2 |= (0x80 << 8);

	/* PendSV low */
	SCB->shpr3 |= (0xf0);

	/* We can do ticker initialization later */

	/*
	 * Since we aren't currently using external interrupts, we don't need
	 * NVIC at all
	 */

}
