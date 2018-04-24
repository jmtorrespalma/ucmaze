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

#include <config.h>
#include <dev.h>
#include <uart.h>
#include <ticker.h>

extern struct uart_dev uart_std;

/*
 * Initialization of default devices, uart_std and system ticker.
 */
void dev_init(void)
{
	struct uart_conf std_conf = {.status = UART_STATUS_EN,
				     .baudrate = TTY_BR};

	uart_set_config(&uart_std, &std_conf);
	ticker_init(TICKER_FREQ);
}
