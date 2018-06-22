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

#include <stdint.h>
#include <board.h>
#include <ticker.h>
#include "riscv.h"

extern unsigned int jiffies;
uint64_t reload;

void ticker_init(uint32_t freq)
{
	reload = LFROSC_FREQ / (uint64_t)freq;

	CLINT->mtimecmp = reload;
	set_csr(mie, MIE_MTIE);
}

/*
 * Riscv timer needs to be restored in every cycle.
 */
void ticker_reset(void)
{
	CLINT->mtimecmp = reload;
}

#endif /* TICKER_H_ */
