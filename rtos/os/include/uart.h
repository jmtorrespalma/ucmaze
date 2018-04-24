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

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/*
 * Following constants are used in the field status to or them and keep more
 * compact data, so from there we can extract information much better.
 * XXX: use bitfields.
 */
#define UART_STATUS_EN (1 << 1)

/*
 * Used to define a set of capabilities by the OS, so we are aware of what we
 * can do with it at execution time in a higher level.
 */
enum uart_type {
	UART_BASIC = 0, /* Basic blocking interrupts */
	UART_TRINTS,    /* Supports asynchronous i/o with interrupts */
	UART_UNKNOWN    /* Unknown device type */
};

/*
 * Configuration of uart at system/app level.
 */
struct uart_conf {
	unsigned int status;
	unsigned int baudrate;
};

/*
 * Generic uart device
 * These devices should always be fixed at compiled time, we don't have any
 * support for hotplug or autodetect devices.
 */
struct uart_dev {
	uint32_t id;
	const enum uart_type type;
	struct uart_conf conf;
};

int uart_set_config(struct uart_dev *d, struct uart_conf *c);
int uart_get_config(struct uart_dev *d, struct uart_conf *c);
int uart_send_byte(struct uart_dev *d, uint8_t b);
int uart_read_byte(struct uart_dev *d, uint8_t *b);

#endif /* UART_H_ */
