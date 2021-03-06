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

#include <stdint.h>
#include <config.h>
#include "dev-def.h"

/*
 * Primary interrupt controller driver.
 */

extern void ticker_handler(void);

void ints_init(void)
{
	PIC_BASE->irqes |= (1 << TIMER1_PID);
}

/*
 * Called from the low level interrupt handler, here we determine the source
 * and call the assignated isr for that interrupt type.
 *
 * Peripherals should internally clear their own interrupt in the peripheral
 * driver interrupt handler.
 */
void _soc_irq_handler(void)
{
	/* FIXME: Only timer interrupt implemented */
	if (PIC_BASE->irqs & (1 << TIMER1_PID)) {
		PIC_BASE->irqec = (1 << TIMER1_PID);
		ticker_handler();
		TIMER1_BASE->iclr = 1;
		PIC_BASE->irqes |= (1 << TIMER1_PID);
	}
}
