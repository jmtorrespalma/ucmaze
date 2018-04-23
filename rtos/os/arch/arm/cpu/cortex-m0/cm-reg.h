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

#ifndef CM_REG_H_
#define CM_REG_H_

#include <stdint.h>

/*
 * These registers are only word accessible so using bitfields or other types
 * is not recommended.
 */

struct _systick {
	uint32_t csr;
	uint32_t rvr;
	uint32_t cvr;
	uint32_t calib;
};
#define SYSTICK ((volatile struct _systick *)0xe000e010)

struct _nvic {
	uint32_t iser;
	uint8_t rsv0[124];
	uint32_t icer;
	uint8_t rsv1[124];
	uint32_t ispr;
	uint8_t rsv2[124];
	uint32_t icpr;
	uint8_t rsv3[636];
	uint32_t ipr[8];
};
#define NVIC ((volatile struct _nvic *)0xe000e100)

struct _scb {
	uint32_t cpuid;
	uint32_t icsr;
	uint8_t rsv0[4];
	uint32_t aircr;
	uint32_t scr;
	uint32_t ccr;
	uint8_t rsv1[4];
	uint32_t shpr2;
	uint32_t shpr3;
};
#define SCB ((volatile struct _scb *)0xe000ed00)

#endif /* CM_REG_H_ */
