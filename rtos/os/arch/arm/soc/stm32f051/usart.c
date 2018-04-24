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

#define uart2usart(_d) ((struct stm32_usart *)(_d)->id)
#define TX_EMPTY(_u) ((_u)->isr | (1 << 7))
#define TX_SENT(_u) ((_u)->isr | (1 << 6))
#define RX_NEMPTY(_u) ((_u)->isr | (1 << 6))

void stm32_usart_enable(volatile struct stm32_usart *u)
{
	u->cr1 |= 0xd; /* Enable tx, rx and usart */
}

void stm32_usart_disable(volatile struct stm32_usart *u)
{
	u->cr1 &= ~(0xd);
}

void stm32_usart_send_byte(volatile struct stm32_usart *u, uint8_t b)
{
	while (!TX_EMPTY(u));
	u->tdr = b;
	while (!TX_SENT(u));
}

uint8_t stm32_usart_read_byte(volatile struct stm32_usart *u)
{
	while (!RX_NEMPTY(u));
	return u->rdr;
}

/*
 * It depends on oversampling, 16 bit by default.
 * BRR = clock_freq / baudrate.
 */
void stm32_usart_set_baudrate(volatile struct stm32_usart *u,
			      unsigned int baudrate)
{
	u->brr = CPU_FREQ / baudrate;
}

/*
 * Stubs for kernel implementation
 */
int _soc_uart_send_byte(struct uart_dev *d, uint8_t b)
{
	volatile struct stm32_usart *lld = uart2usart(d);

	stm32_usart_send_byte(lld, b);

	return 0;
}

int _soc_uart_read_byte(struct uart_dev *d, uint8_t *b)
{
	volatile struct stm32_usart *lld = uart2usart(d);

	*b = stm32_usart_read_byte(lld);

	return 0;
}

/*
 * Order is important in this one.
 */
int _soc_uart_set_config(struct uart_dev *d, struct uart_conf *c)
{
	volatile struct stm32_usart *lld = uart2usart(d);

	stm32_usart_disable(lld);
	stm32_usart_set_baudrate(lld, c->baudrate);

	d->conf.status = c->status;
	d->conf.baudrate = c->baudrate;

	if ((c->status & UART_STATUS_EN))
		stm32_usart_enable(lld);

	return 0;
}
