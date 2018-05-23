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
#include <util/rbuf.h>

/*
 * Following constants are used in the field status to or them and keep more
 * compact data, so from there we can extract information much better.
 * We don't use bitfields for being problematic across architectures.
 */
#define UART_ENABLE  (1)
#define UART_DISABLE (0)

#define UART_STATUS  (1 << 0) /* Device status (on/off) */
#define UART_IRQ_TX  (1 << 1) /* Tx interrupt */
#define UART_IRQ_RX  (1 << 2) /* Rx interrupt */

/*
 * Used to select if sending/receiving is blocking or not.
 */
#define NO_BLOCK 0
#define BLOCK    1

/*
 * Used to define a set of capabilities by the OS, so we are aware of what we
 * can do with it at execution time in a higher level.
 */
enum uart_type {
	UART_BASIC = 0,   /* Basic blocking interrupts */
	UART_TRX_ASYNC,   /* Supports asynchronous i/o with interrupts */
	UART_UNKNOWN      /* Unknown device type */
};

/*
 * Configuration of uart at system/app level.
 * TODO: add stop bits, parity, etc.
 */
struct uart_conf {
	unsigned int status;
	unsigned int baudrate;
};

/*
 * Generic uart device.
 */
struct uart_dev {
	uint32_t id;
	const enum uart_type type;
	struct uart_conf conf;
	struct rbuf *rcv_buf;
	struct rbuf *snd_buf;
};

/*
 * Macros to simplify code ahead.
 */
#define UART_DEV_ON(_d)  ((_d)->conf.status & UART_STATUS)
#define UART_DEV_OFF(_d) (!UART_DEV_ON((_d)))

#define UART_CONF_IRQ_ON(_c)  ((_c)->status & UART_IRQ_TX || \
			       (_c)->status & UART_IRQ_RX)
#define UART_CONF_IRQ_OFF(_c) (!UART_CONF_IRQ_ON((_c))

int uart_set_config(struct uart_dev *d, struct uart_conf *c);
int uart_get_config(struct uart_dev *d, struct uart_conf *c);

int uart_send_byte(struct uart_dev *d, uint8_t b, int mode);
int uart_receive_byte(struct uart_dev *d, uint8_t *b, int mode);

int uart_send_bytes(struct uart_dev *d, uint8_t *b, int mode);
int uart_receive_bytes(struct uart_dev *d, uint8_t *b, int mode);

#endif /* UART_H_ */
