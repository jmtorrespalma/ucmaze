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

#include <stdlib.h>
#include <kstdio.h>
#include <uart.h>

extern struct uart_dev uart_std;

int kgetchar(void)
{
	int rv;
	uint8_t r;

	rv = uart_read_byte(&uart_std, &r);
	if (rv)
		return EOF;

	return (int)r;
}

int kputchar(int c)
{
	int rv;

	rv = uart_send_byte(&uart_std, (uint8_t)c);
	if (rv)
		return EOF;

	return 0;
}

char *kgets(char *str)
{
	uint8_t r;
	int rv;

	do {
		rv = uart_read_byte(&uart_std, &r);
		*str++ = (char)r;
	} while (r != '\n' && !rv);

	*--str = '\0';

	if (rv)
		return NULL;

	return str;
}

int kputs(const char *str)
{
	uint8_t r;
	int rv;

	do {
		r = (uint8_t)*str++;
		rv = uart_send_byte(&uart_std, r);
	} while (r != '\0' && !rv);

	if (rv)
		return EOF;

	rv = uart_send_byte(&uart_std, '\n');
	if (rv)
		return EOF;

	return 0;
}
