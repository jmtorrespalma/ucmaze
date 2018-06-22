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

#ifndef DEV_DEF_H_
#define DEV_DEF_H_

/*
 * fe310 device definitions
 */

/*
 * UART included in FE310
 */
struct fe310_uart {
	uint32_t txdata; /* Transmit data register */
	uint32_t rxdata; /* Receive data register */
	uint32_t txctrl; /* Transmit control register */
	uint32_t rxctrl; /* Receive control register */
	uint32_t ie;     /* Interrupt enable */
	uint32_t ip;     /* Interrupt pending */
	uint32_t div;    /* Baudrate divisor */
};

#define UART0_BASE   ((volatile struct fe310_uart *)0x10013000)
#define UART1_BASE   ((volatile struct fe310_uart *)0x10023000)


#endif /* DEV_DEF_H_ */
