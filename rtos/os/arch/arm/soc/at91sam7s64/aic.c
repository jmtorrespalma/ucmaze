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
 * This is the driver for the advanced interrupt controller.
 */

#define FIQ_PID  0
#define SYSC_PID 1

extern void ticker_handler(void);

/*
 * Using vectored interrupt mode, set which handler will be returned by the AIC
 * when the interrupt n happens.
 */
void sam7_aic_set_handler(int n, void(*handler)(void))
{
	AIC_BASE->smr[n] = (uint32_t)handler;
}

void sam7_aic_enable(int n)
{
	AIC_BASE->iecr |= (1 << n);
}

/*
 * 3 bit values, being 0 the lowest and 7 the highest priority.
 */
void sam7_aic_set_priority(int n, int prio)
{
	AIC_BASE->smr[n] &= ~0x3;
	AIC_BASE->smr[n] |= (uint32_t)prio;
}

/*
 * Called from the low level interrupt handler, here we determine the source
 * and call the assignated isr for that interrupt type.
 */
void _soc_irq_handler(void)
{
	void (*handler)(void);
	/*
	 * By reading ivr, aic is notified that the cpu acknowledged the
	 * interrupt.
	 */
	handler = (void *)AIC_BASE->ivr;
	handler();

	/*
	 * Before exiting we need to notify that the interrupt was serviced and
	 * can go back to normal or to another lower priority interrupt.
	 */
	AIC_BASE->eoicr = 1;
}

/*
 * Handler for the system controller peripheral interrupt, shared by several
 * devices such as the ticker, adc, etc.
 *
 * TODO: ask the devices one by one to check which of them caused the
 * interrupt.
 */
void _sysc_handler(void)
{
	uint32_t unused;

	ticker_handler();
	unused = PIT_BASE->pivr; /* Clear the interrupt */
	(void)unused;
}

/*
 * Initializes the interrupt controller with default parameters, ticker
 * interrupt at max priority.
 */
void ints_init(void)
{
	sam7_aic_set_handler(SYSC_PID, _sysc_handler);
	sam7_aic_set_priority(SYSC_PID, 7);
	sam7_aic_enable(SYSC_PID);
}
