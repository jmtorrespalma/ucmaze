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

#define uart2usart(_d) ((struct sam7_usart *)(_d)->id)
#define TX_EMPTY(_u) ((_u)->csr & (1 << 9))
#define TX_SENT(_u) ((_u)->csr & (1 << 4))
#define RX_NEMPTY(_u) ((_u)->csr & (1 << 0))

void sam7_usart_enable(volatile struct sam7_usart *u)
{
	u->cr |= 0x50; /* TXEN and RXEN */
}

void sam7_usart_disable(volatile struct sam7_usart *u)
{
	u->cr |= 0xa0;
}

void sam7_usart_send_byte(volatile struct sam7_usart *u, uint8_t b)
{
	while (!TX_EMPTY(u));
	u->thr = b;
	while (!TX_SENT(u));
}

uint8_t sam7_usart_read_byte(volatile struct sam7_usart *u)
{
	while (!RX_NEMPTY(u));
	return u->rhr;
}

/*
 * Based on master clock (MCK).
 */
void sam7_usart_set_baudrate(volatile struct sam7_usart *u,
			     unsigned int baudrate)
{
	u->brgr = CPU_FREQ / (baudrate);
}

/*
 * Stubs for kernel implementation
 */
int _soc_uart_send_byte(struct uart_dev *d, uint8_t b)
{
	volatile struct sam7_usart *lld = uart2usart(d);

	sam7_usart_send_byte(lld, b);

	return 0;
}

int _soc_uart_read_byte(struct uart_dev *d, uint8_t *b)
{
	volatile struct sam7_usart *lld = uart2usart(d);

	*b = sam7_usart_read_byte(lld);

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
	volatile struct sam7_usart *lld = uart2usart(d);

	sam7_usart_disable(lld);

	/*
	 * TODO: read data size and stop bits from config.
	 * Set SYNC register too.
	 */
	lld->mr = ((3 << 6) | (4 << 9) | (1 << 8));
	sam7_usart_set_baudrate(lld, c->baudrate);


	d->conf.status = c->status;
	d->conf.baudrate = c->baudrate;

	if ((c->status & UART_STATUS_EN))
		sam7_usart_enable(lld);

	return 0;
}
