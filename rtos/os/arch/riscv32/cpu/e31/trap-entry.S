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

#include "riscv.h"

/*
 * Low level trap handler. This routine is called under every interrupt or
 * syscall triggering, so basically is the kernel entry point for riscv.
 *
 * First we need to save the whole context in the stack, followed by interrupt
 * or exception identification
 */

	.section .init, "ax"
	.global _trap_handler
	.func   _trap_handler
_trap_handler:
	csrrci	x0, mstatus, MSTATUS_MIE

	sw	x1, -4(sp)
	sw	x5, -8(sp)
	sw	x6, -12(sp)
	sw	x7, -16(sp)
	sw	x8, -20(sp)
	sw	x9, -24(sp)
	sw	x10, -28(sp)
	sw	x11, -32(sp)
	sw	x12, -36(sp)
	sw	x13, -40(sp)
	sw	x14, -44(sp)
	sw	x15, -48(sp)
	sw	x16, -52(sp)
	sw	x17, -56(sp)
	sw	x18, -60(sp)
	sw	x19, -64(sp)
	sw	x20, -68(sp)
	sw	x21, -72(sp)
	sw	x22, -76(sp)
	sw	x23, -80(sp)
	sw	x24, -84(sp)
	sw	x25, -88(sp)
	sw	x26, -92(sp)
	sw	x27, -96(sp)
	sw	x28, -100(sp)
	sw	x29, -104(sp)
	sw	x30, -108(sp)
	sw	x31, -112(sp)

	csrr	a0, mstatus
	csrr	a1, mepc
	sw	a1, -116(sp)
	sw	a0, -120(sp)
	addi	sp, sp, -120

	/*
	 * Change to kernel stack.
	 */
	csrrw	sp, mscratch, sp

	/*
	 * Check if the call was a interrupt or not.
	 */
	csrr	a0, mcause
	jal	trap_handler

	/*
	 * Return to caller stack.
	 */
	csrrw	sp, mscratch, sp

	/*
	 * Restore task context
	 */
	sw	a0, (sp)
	csrw	mstatus, a0
	sw	a0, 4(sp)
	csrw	mepc, a0
	addi	sp, sp, 120

	lw	x1, -4(sp)
	lw	x5, -8(sp)
	lw	x6, -12(sp)
	lw	x7, -16(sp)
	lw	x8, -20(sp)
	lw	x9, -24(sp)
	lw	x10, -28(sp)
	lw	x11, -32(sp)
	lw	x12, -36(sp)
	lw	x13, -40(sp)
	lw	x14, -44(sp)
	lw	x15, -48(sp)
	lw	x16, -52(sp)
	lw	x17, -56(sp)
	lw	x18, -60(sp)
	lw	x19, -64(sp)
	lw	x20, -68(sp)
	lw	x21, -72(sp)
	lw	x22, -76(sp)
	lw	x23, -80(sp)
	lw	x24, -84(sp)
	lw	x25, -88(sp)
	lw	x26, -92(sp)
	lw	x27, -96(sp)
	lw	x28, -100(sp)
	lw	x29, -104(sp)
	lw	x30, -108(sp)
	lw	x31, -112(sp)

	mret

        .size   _trap_handler, . - _trap_handler
        .endfunc
