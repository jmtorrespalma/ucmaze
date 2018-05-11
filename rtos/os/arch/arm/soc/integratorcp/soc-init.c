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

#include <config.h>
#include "dev-def.h"

/*
 * The qemu simulator requires the vector table to be copied to the beginning
 * of RAM, otherwise can't respond to exceptions.
 */
void copy_handlers(void)
{
	uint32_t *it = 0x0, *src = (void *)0x10000;

	for (int i = 0; i < 16; ++i)
		it[i] = src[i];

}

/*
 * Only ticker interrupt is enabled.
 */
extern void ints_init(void);

/*
 * Ticker is implemented using timer 1, whose input clock is 1 MHz.
 * We use it in periodic mode.
 */
#define TICKER_CLOCK (1000000u)
void ticker_init(uint32_t period)
{
	TIMER1_BASE->ctl |= (1 << 1); /* 32 bits timer size */
	TIMER1_BASE->ctl |= (1 << 5); /* Interrupt enable */
	TIMER1_BASE->ctl |= (1 << 6); /* Periodic mode */

	/* Reload value */
	TIMER1_BASE->lv = TICKER_CLOCK / period;

	TIMER1_BASE->ctl |= (1 << 7); /* Enable bit */
}


int soc_init(void)
{
	copy_handlers();
	ints_init();

	return 0;
}
