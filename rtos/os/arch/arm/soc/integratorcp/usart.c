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
#include <config.h>
#include <uart.h>
#include "dev-def.h"

#define uart2icp_uart(_d) ((struct icp_uart *)(_d)->id)
#define ICP_UART_FULL(_u) ((_u)->fr & (1 << 5))

/*
 * Stubs for kernel implementation
 */
int _soc_uart_send_byte(struct uart_dev *d, uint8_t b)
{
	struct icp_uart *iu = uart2icp_uart(d);

	while (ICP_UART_FULL(iu));
	iu->dr = b;

	return 0;
}

int _soc_uart_read_byte(struct uart_dev *d, uint8_t *b)
{
	struct icp_uart *iu = uart2icp_uart(d);

	while (ICP_UART_FULL(iu));
	*b = iu->dr ;

	return 0;
}

/*
 * Default configuration:
 * - br: 9600 baud
 * - data: 8b
 * - parity: none
 * - stop: 1b
 * - flow: none
 */
int _soc_uart_set_config(struct uart_dev *d, struct uart_conf *c)
{
	struct icp_uart *iu = uart2icp_uart(d);

	d->conf.status = c->status;
	d->conf.baudrate = c->baudrate;

	return 0;
}
