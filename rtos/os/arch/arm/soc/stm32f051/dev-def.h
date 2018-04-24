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
 * stm32f051 device definitions
 */

/*
 * Interesting available interrupts are:
 * - TXE: transmit register empty.
 * - TC: transmission complete.
 * - RXNE: receive register not empty.
 * - CMF: character match.
 */
struct stm32_usart {
	uint32_t cr1; /* Control and enable */
	uint32_t cr2;
	uint32_t cr3;
	uint32_t brr; /* Baudrate */
	uint32_t gtpr;
	uint32_t rtor;
	uint32_t rqr;
	uint32_t isr; /* Interrupt status */
	uint32_t icr; /* Interrupt clear */
	uint32_t rdr; /* Read register */
	uint32_t tdr; /* Transmit register */
};

#define USART1_BASE   ((volatile struct stm32_usart *)0x40013800)
#define USART2_BASE   ((volatile struct stm32_usart *)0x40004400)


#endif /* DEV_DEF_H_ */
