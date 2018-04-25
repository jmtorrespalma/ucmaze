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
#include <kstdio.h>

/* GPIO defines */
#define GPIO_BASE        ((volatile uint32_t *)0x48000800)
#define GPIO_ODR         ((volatile uint32_t *)0x48000814)
#define PIN_BLUE         8
#define DELAY_BLUE       0x080000
#define PIN_GREEN        9
#define DELAY_GREEN      0x040000
#define DELAY_MAIN       0x100000

unsigned int argv_green[] = {PIN_GREEN, DELAY_GREEN};
unsigned int argv_blue[] = {PIN_BLUE, DELAY_BLUE};

int toggle_led(int argc, unsigned int *argv)
{
	unsigned int pin, delay;

	pin = argv[0];
	delay = argv[1];

	/* Set pin as output */
	*GPIO_BASE |= (1 << (pin * 2));

	while (1) {
		*GPIO_ODR ^= 1 << pin;
		for (int i = 0; i < delay; ++i);
	}
}

int main(void)
{
	char msg[32];

	/* Create both threads */
	task_create(20, toggle_led, 2, argv_green);
	task_create(20, toggle_led, 2, argv_blue);

	/* Test stdout */
	kputs("Input message and end with LF");
	kgets(msg);
	kputs(msg);

	while (1);

	return 0;
}
