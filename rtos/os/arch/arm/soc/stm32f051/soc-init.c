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

/*
 * stm32f0 cores are clock gated, so that means all peripherals clocks are off
 * upon reset. We have to manually set them on if we want to use them.
 */

/* Clock defines */
#define RCC_BASE   ((volatile uint32_t *)0x40021000)
#define RCC_AHBENR ((volatile uint32_t *)0x40021014)
#define RCC_APBENR ((volatile uint32_t *)0x40021018)
#define IOPCEN     ((uint32_t)(1 << 19))
#define USART1EN   ((uint32_t)(1 << 14))

/* Pins for usart1 */
#define GPIOA_MODER ((volatile uint32_t *)0x48000000)
#define TX_PIN 9
#define RX_PIN 10

int soc_init(void)
{
	/* Enable clock for gpio C */
	*RCC_AHBENR = IOPCEN;

	/* Set pins and clock for usart 1 */
	*RCC_APBENR = USART1EN;
	*GPIOA_MODER |= (0x2 << (TX_PIN * 2));
	*GPIOA_MODER |= (0x2 << (RX_PIN * 2));

	return 0;
}
