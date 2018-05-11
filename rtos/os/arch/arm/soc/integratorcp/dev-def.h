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

#include <stdint.h>

/*
 * Peripheral map definition.
 */

struct icp_timer {
	uint32_t lv;       /* Load value */
	uint32_t cv;       /* Current value */
	uint32_t ctl;      /* Control */
	uint32_t iclr;     /* Interrupt clear */
	uint32_t ris;      /* Raw interrupt status */
	uint32_t mis;      /* Masked interrupt status */
	uint32_t bgl;      /* Background load value */
};

/*
 * Primary interrupt controller
 */
struct icp_pic {
	uint32_t irqs;    /* IRQ status */
	uint32_t irqrs;   /* IRQ raw status */
	uint32_t irqes;   /* IRQ enable set */
	uint32_t irqec;   /* IRQ enable clear */
	uint32_t softs;   /* Software interrupt set */
	uint32_t softc;   /* Software interrupt clear */
	uint32_t fiqs;    /* FIQ status */
	uint32_t fiqrs;   /* FIQ raw status */
	uint32_t fiqes;   /* FIQ enable set */
	uint32_t fiqec;   /* FIQ enable clear */
};

struct icp_uart {
	uint32_t dr;       /* Data read or written */
	uint32_t rsr_ecr;  /* Receive status or write error */
	uint32_t rsv0[4];
	uint32_t fr;       /* Flags register */
	uint32_t rsv1;
	uint32_t ilpr;     /* IrDA */
	uint32_t ibrd;     /* Integer baudrate divisor */
	uint32_t fbrd;     /* Fractional baudrate divisor */
	uint32_t lcrh;     /* Line control high */
	uint32_t cr;       /* Control register */
	uint32_t ifls;     /* Interrupt FIFO level selector */
	uint32_t imsc;     /* Interrupt mask set/clear */
	uint32_t ris;      /* Raw interrupt status */
	uint32_t mis;      /* Masked interrupt status */
	uint32_t icr;      /* Interrupt clear register */
	uint32_t dmacr;    /* DMA control register */
	uint32_t rsv2[40];

	uint32_t pid0;     /* Peripheral id 0 */
	uint32_t pid1;     /* Peripheral id 1 */
	uint32_t pid2;     /* Peripheral id 2 */
	uint32_t pid3;     /* Peripheral id 3 */

	uint32_t cid0;     /* Cell id 0 */
	uint32_t cid1;     /* Cell id 1 */
	uint32_t cid2;     /* Cell id 2 */
	uint32_t cid3;     /* Cell id 3 */
};

/*
 * Peripheral base addresses
 */
#define TIMER0_BASE  ((volatile struct icp_timer *)0x13000000)
#define TIMER1_BASE  ((volatile struct icp_timer *)0x13000100)
#define TIMER2_BASE  ((volatile struct icp_timer *)0x13000200)
#define PIC_BASE     ((volatile struct icp_pic *)0x14000000)
#define UART0_BASE   ((volatile struct icp_uart *)0x16000000)
#define UART1_BASE   ((volatile struct icp_uart *)0x17000000)

/*
 * Peripheral ids, required for interrupt identification.
 */
#define UART0_PID   (1)
#define UART1_PID   (2)
#define TIMER0_PID  (5)
#define TIMER1_PID  (6)
#define TIMER2_PID  (7)

#endif /* DEV_DEF_H_ */
