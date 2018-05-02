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

#ifndef ARM_REG_H_
#define ARM_REG_H_

/*
 * Bits in cpsr.
 */
#define F_BIT        (1 << 6)
#define I_BIT        (1 << 7)
#define USR_MASK     (0x10)
#define FIQ_MASK     (0x11)
#define IRQ_MASK     (0x12)
#define SVC_MASK     (0x13)
#define ABT_MASK     (0x17)
#define UND_MASK     (0x1b)
#define SYS_MASK     (0x1f)

#endif /* ARM_REG_H_ */
