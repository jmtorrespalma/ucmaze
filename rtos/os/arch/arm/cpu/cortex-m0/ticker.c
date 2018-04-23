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

#ifndef TICKER_H_
#define TICKER_H_

#include <ticker.h>
#include <config.h>
#include "cm-reg.h"

void ticker_init(uint32_t freq)
{
	uint32_t reload;

	reload = CPU_FREQ / freq;

	SYSTICK->cvr = 0x0;
	SYSTICK->rvr = reload;
	SYSTICK->csr |= 3; /* Enable ticker and interrupt */
}

#endif /* TICKER_H_ */
