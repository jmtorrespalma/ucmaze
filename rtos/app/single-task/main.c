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
#define PIO_PER          ((volatile uint32_t *)0xfffff400)
#define PIO_OER          ((volatile uint32_t *)0xfffff410)
#define PIO_SODR         ((volatile uint32_t *)0xfffff430)
#define PIO_CODR         ((volatile uint32_t *)0xfffff434)
#define PIN_YELLOW       17
#define DELAY_YELLOW     0x080000
#define PIN_GREEN        18
#define DELAY_GREEN      0x040000

unsigned int argv_green[] = {PIN_GREEN, DELAY_GREEN};
unsigned int argv_yellow[] = {PIN_YELLOW, DELAY_YELLOW};

int toggle_led(int argc, unsigned int *argv)
{
	unsigned int pin, delay;

	pin = argv[0];
	delay = argv[1];

	/* Set pin as output */
	*PIO_PER = (1 << (pin));
	*PIO_OER = (1 << (pin));

	while (1) {
		*PIO_SODR = (1 << pin);
		for (int i = 0; i < delay; ++i);
		*PIO_CODR = (1 << pin);
		for (int i = 0; i < delay; ++i);
	}
}

int main(void)
{
	char msg[] = "Welcome to ucmaze";

	/* Create both threads */
	task_create(20, toggle_led, 2, argv_green);
	task_create(20, toggle_led, 2, argv_yellow);

	/* Test stdout */
	kputs(msg);

	return 0;
}
