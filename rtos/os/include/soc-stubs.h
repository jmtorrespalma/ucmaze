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

#ifndef SOC_STUBS_H_
#define SOC_STUBS_H_

/*
 * List of stubs that need to be implemented by new socs or drivers for the
 * kernel to be able to link with the low level driver of each device.
 */

#include <uart.h>

int _soc_uart_set_config(struct uart_dev *d, struct uart_conf *c);
int _soc_uart_send_byte(struct uart_dev *d, uint8_t b);
int _soc_uart_read_byte(struct uart_dev *d, uint8_t *b);

#endif /* SOC_STUBS_H_ */
