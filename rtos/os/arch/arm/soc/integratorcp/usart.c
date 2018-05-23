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
int _soc_uart_write_byte(struct uart_dev *d, uint8_t b)
{
	volatile struct icp_uart *iu = uart2icp_uart(d);

	iu->dr = b;

	return 0;
}

int _soc_uart_read_byte(struct uart_dev *d, uint8_t *b)
{
	volatile struct icp_uart *iu = uart2icp_uart(d);

	*b = iu->dr ;

	return 0;
}

/*
 *
 */
int _soc_uart_set_config(struct uart_dev *d, struct uart_conf *c)
{
	volatile struct icp_uart *iu = uart2icp_uart(d);

	(void)iu;
	d->conf.status = c->status;
	d->conf.baudrate = c->baudrate;

	return 0;
}

int _soc_uart_tx_empty(struct uart_dev *d)
{
	volatile struct icp_uart *iu = uart2icp_uart(d);
	return (!ICP_UART_FULL(iu));
}

/*
 * Sends instantly, it's an emulator.
 */
int _soc_uart_tx_finished(struct uart_dev *d)
{
	volatile struct icp_uart *iu = uart2icp_uart(d);
	(void)iu;
	return 1;
}

int _soc_uart_rx_empty(struct uart_dev *d)
{
	volatile struct icp_uart *iu = uart2icp_uart(d);
	return (!ICP_UART_FULL(iu));
}
