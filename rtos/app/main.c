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
#include <ucmaze-os.h>

/* GPIO defines */
#define GPIO_BASE  ((volatile uint32_t *)0x48000800)
#define GPIO_ODR   ((volatile uint32_t *)0x48000814)
#define PIN_BLUE   8
#define PIN_GREEN  9
#define DELAY      0x100000

int toogle_blue_led(void)
{
	while (1) {
		*GPIO_ODR ^= 1 << PIN_BLUE;
		for (int i = 0; i < DELAY; ++i);
	}
}

int toogle_green_led(void)
{
	while (1) {
		*GPIO_ODR ^= 1 << PIN_GREEN;
		for (int i = 0; i < DELAY; ++i);
	}
}

int main(void)
{

	/* Set pins as output */
	*GPIO_BASE |= (1 << (PIN_BLUE * 2));
	*GPIO_BASE |= (1 << (PIN_GREEN * 2));

	/* Create both threads */
	task_create(20, toogle_blue_led, 0, 0);
	task_create(20, toogle_green_led, 0, 0);

	return 0;
}
