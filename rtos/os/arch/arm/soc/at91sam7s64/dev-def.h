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
 * at91sam7s64 devices and base addresses.
 */

/*
 * Clock is extremely complex for this micro, having 3 sources and different
 * prescalers and oscilators:
 * - RCOscilator -> SLCK = 32kHz, is the only constant clock.
 * - Main oscilator, external source of around 18 MHz.
 * - PLL, allows to divide and multiply main oscilator.
 *
 * Any of the previous can be used to set the MCK or master clock of the
 * system.
 */
struct sam7_pmc {
	uint32_t scer; /* System clock */
	uint32_t scdr;
	uint32_t scsr;
	uint32_t rsv0;
	uint32_t pcer; /* Peripheral clock */
	uint32_t pcdr;
	uint32_t pcsr;
	uint32_t rsv1;
	uint32_t mor;  /* Main oscillator */
	uint32_t mcfr; /* Main clock */
	uint32_t rsv2;
	uint32_t pllr; /* PLL */
	uint32_t mckr; /* Master clock */
	uint32_t rsv3[3];
	uint32_t pck0; /* Programmable clocks */
	uint32_t pck1;
	uint32_t pck2;
	uint32_t rsv5[5];
	uint32_t ier;  /* Interrupt registers */
	uint32_t idr;
	uint32_t sr;
	uint32_t imr;
	uint32_t rsv6[4];
};

#define PMC_BASE   ((volatile struct sam7_pmc *)0xfffffc00)

/*
 * Periodic interval timer, used as system ticker.
 */
struct sam7_pit {
	uint32_t mr;
	uint32_t sr;
	uint32_t pivr;
	uint32_t piir;
};

#define PIT_BASE   ((volatile struct sam7_pit *)0xfffffd30)

/*
 * Interrupt controller.
 */
struct sam7_aic {
	uint32_t smr[32]; /* Mode register */
	uint32_t svr[32]; /* Vector register, address for handler */
	uint32_t ivr;     /* Current interrupt vector */
	uint32_t fvr;     /* Vector for FIQ */
	uint32_t isr;     /* Interrupt status */
	uint32_t ipr;     /* Interrupt pending */
	uint32_t imr;     /* Interrupt mask */
	uint32_t cisr;
	uint32_t rsv0[2];
	uint32_t iecr;    /* Enable command interrupt */
	uint32_t idcr;    /* Disable command interrupt */
	uint32_t iccr;    /* Clear command interrupt */
	uint32_t iscr;    /* Set command interrupt */
	uint32_t eoicr;   /* End of command interrupt */
	uint32_t spu;     /* Spurious interrupt vector */
	uint32_t dcr;     /* Debug */
	uint32_t rsv1;
	uint32_t ffer;
	uint32_t ffdr;
	uint32_t ffsr;
};

#define AIC_BASE   ((volatile struct sam7_aic *)0xfffff000)

#endif /* DEV_DEF_H_ */
