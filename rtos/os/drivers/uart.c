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
#include <util/rbuf.h>

/*
 * Sends a single byte.
 */
int uart_send_byte(struct uart_dev *d, uint8_t b, int mode)
{
	if (UART_DEV_OFF(d))
		return -EINVAL;

	if (mode == BLOCK)
		while (!_soc_uart_tx_empty(d));
	else
		if (!_soc_uart_tx_empty(d))
			return -EBUSY;

	_soc_uart_write_byte(d, b);

	if (mode == BLOCK)
		while (!_soc_uart_tx_finished(d));

	return 0;
}

/*
 * Receives a single byte.
 */
int uart_receive_byte(struct uart_dev *d, uint8_t *b, int mode)
{
	if (UART_DEV_OFF(d))
		return -EINVAL;

	if (mode == BLOCK)
		while (_soc_uart_rx_empty(d));
	else
		if (_soc_uart_rx_empty(d))
			return -EAGAIN;

	_soc_uart_read_byte(d, b);

	return 0;
}

int uart_set_config(struct uart_dev *d, struct uart_conf *c)
{
	if (UART_CONF_IRQ_ON(c) && d->type == UART_BASIC)
		return -EINVAL;

	return _soc_uart_set_config(d, c);
}

int uart_get_config(struct uart_dev *d, struct uart_conf *c)
{
	c->status = d->conf.status;
	c->baudrate = d->conf.baudrate;

	return 0;
}

/*
 * Called when a receive interrupt happens to uart.
 */
//int uart_receive_handler(struct uart_dev *d)
//{
//	uint8_t c;
//	int rv;
//
//	if (d->type != UART_TRINTS)
//		return -EINVAL;
//
//	_soc_uart_receive_byte(d, &c);
//	rbuf_push(r->rcv_buf, &c);
//
//	if (rbuf_full(r->rcv_buf))
//		_soc_uart_int_ctl(UART_IRQ_RX, IRQ_DIS);
//
//	return 0;
//}
//
//int uart_send_handler(struct uart_dev *d)
//{
//
//}
