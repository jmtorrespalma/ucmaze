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

#include <errno.h>
#include <soc-stubs.h>
#include <uart.h>

int uart_send_byte(struct uart_dev *d, uint8_t b)
{
	if (!(d->conf.status & UART_STATUS_EN))
		return -EINVAL;

	return _soc_uart_send_byte(d, b);
}

int uart_read_byte(struct uart_dev *d, uint8_t *b)
{
	if (!(d->conf.status & UART_STATUS_EN))
		return -EINVAL;

	return _soc_uart_read_byte(d, b);
}

int uart_set_config(struct uart_dev *d, struct uart_conf *c)
{
	return _soc_uart_set_config(d, c);
}

int uart_get_config(struct uart_dev *d, struct uart_conf *c)
{
	c->status = d->conf.status;
	c->baudrate = d->conf.baudrate;

	return 0;
}
