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
#include <board.h>
#include <uart.h>
#include "dev-def.h"

#define uart2uart(_d) ((struct fe310_uart *)(_d)->id)
#define TX_FULL(_u) ((_u)->txdata & (1 << 31))
#define RX_EMPTY(_u) ((_u)->rxdata & (1 << 31))

void fe310_uart_enable(volatile struct fe310_uart *u)
{
	/* Enable tx, rx and usart */
	u->txctrl |= 1;
	u->rxctrl |= 1;
}

void fe310_uart_disable(volatile struct fe310_uart *u)
{
	u->txctrl &= ~(1);
	u->rxctrl &= ~(1);
}

void fe310_uart_write_byte(volatile struct fe310_uart *u, uint8_t b)
{
	u->txdata = b;
}

uint8_t fe310_uart_read_byte(volatile struct fe310_uart *u)
{
	return u->rxdata;
}

/*
 * The clock source is tlclk
 */
void fe310_uart_set_baudrate(volatile struct fe310_uart *u,
			      unsigned int baudrate)
{
	u->div = (CPU_FREQ / baudrate) - 1;
}

/*
 * Stubs for kernel implementation
 */
int _soc_uart_write_byte(struct uart_dev *d, uint8_t b)
{
	volatile struct fe310_uart *lld = uart2uart(d);

	fe310_uart_write_byte(lld, b);

	return 0;
}

int _soc_uart_read_byte(struct uart_dev *d, uint8_t *b)
{
	volatile struct fe310_uart *lld = uart2uart(d);

	*b = fe310_uart_read_byte(lld);

	return 0;
}

/*
 * Order is important in this one.
 */
int _soc_uart_set_config(struct uart_dev *d, struct uart_conf *c)
{
	volatile struct fe310_uart *lld = uart2uart(d);

	fe310_uart_disable(lld);
	fe310_uart_set_baudrate(lld, c->baudrate);

	d->conf.status = c->status;
	d->conf.baudrate = c->baudrate;

	if ((c->status & UART_ENABLE))
		fe310_uart_enable(lld);

	return 0;
}

/*
 * Check if the FIFO is full or not.
 */
int _soc_uart_tx_empty(struct uart_dev *d)
{
	volatile struct fe310_uart *lld = uart2uart(d);
	return !TX_FULL(lld);
}

/*
 * We have no way to check if it's finished or not
 */
int _soc_uart_tx_finished(struct uart_dev *d)
{
	(void)d;
	return 1;
}

int _soc_uart_rx_empty(struct uart_dev *d)
{
	volatile struct fe310_uart *lld = uart2uart(d);
	return RX_EMPTY(lld);
}
