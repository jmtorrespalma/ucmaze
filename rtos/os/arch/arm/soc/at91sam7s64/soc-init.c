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
 * Declaration of interrupt controller initialization.
 */
extern void ints_init(void);

/* Clock defines */
#define MOSCEN   ((uint32_t)(1 << 0))
#define MOSCS    ((uint32_t)(1 << 0))
#define LOCK     ((uint32_t)(1 << 2))
#define MCKRDY   ((uint32_t)(1 << 3))

/* PLL configuration */
#define PLL_MUL  ((uint32_t)(3))
#define PLL_DIV  ((uint32_t)(9))

/*
 * The clock setup is very user unfriendly so we have to be really careful and
 * set everything from here. Our target frequency is 8MHz and the main
 * oscillator frequency is 18MHz, so we need to use PLL.
 *
 * In frequencies over 20MHz, the read wait states of flash should be modified
 * too or it won't be possible to read instructions from flash.
 */
void clock_init(void)
{
	/*
	 * In the slowest case, the main oscillator will be 3MHz, which would
	 * take 16 ms to startup, that would be OSCOUNT = 64 cycles of SLCK.
	 */
	PMC_BASE->mor = ((0x40 << 8) | MOSCEN);
	while (!(PMC_BASE->sr & MOSCS)); /* Wait for stabilization */

	/*
	 * Enable PLL.
	 * PLLCOUNT is the worst case again.
	 */
	PMC_BASE->pllr = ((PLL_MUL << 16) | (16 << 8) | PLL_DIV);
	while (!(PMC_BASE->sr & LOCK));

	/*
	 * Select PLL as master clock.
	 */
	PMC_BASE->mckr |= (0x3);
	while (!(PMC_BASE->sr & MCKRDY));
}

/*
 * Periodic interval timer receives MCLK/16
 * This enables the peripheral, including interrupts.
 */
void ticker_init(uint32_t period)
{
	uint32_t val;

	val = ((CPU_FREQ >> 4) / period) & 0x000fffff; /* 20 bits value */
	PIT_BASE->mr = ((3 << 24) | val);  /* Enable and set value */
}

/*
 * Set pins for usart0 so we can connect to it and clock the device.
 */
static inline void uart_std_preinit(void)
{
	PIOA_BASE->pdr |= (USART0_TX | USART0_RX);
	PIOA_BASE->asr = (USART0_TX | USART0_RX);

	PMC_BASE->pcer |= USART0_PID;
}

/*
 * The soc by default starts with the watchdog enabled so we can run only up to
 * 16 seconds.
 */
#define WDT_MR  ((volatile uint32_t *)0xfffffd44)
void watchdog_disable(void)
{
	*WDT_MR = (1 << 15);
}

int soc_init(void)
{
	clock_init();
	watchdog_disable();
	ints_init();
	uart_std_preinit();

	return 0;
}
