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

#ifndef RISCV_H_
#define RISCV_H_

/*
 * This file can only contain macros, since it's included in assembly too.
 */

/*
 * Word size.
 * Read from gcc macro.
 */
#define XLEN __riscv_xlen

/*
 * Privilege level encodings.
 */
#define RISCV_U (0)
#define RISCV_S (1)
#define RISCV_M (3)

/*
 * Mstatus register.
 */
#define MSTATUS_UIE            (1 << 0) /* User interrupt enable */
#define MSTATUS_SIE            (1 << 1) /* Supervisor interrupt enable */
#define MSTATUS_MIE            (1 << 3) /* Machine interrupt enable */
#define MSTATUS_UPIE           (1 << 4) /* User prior interrupt enable */
#define MSTATUS_SPIE           (1 << 5) /* Supervisor prior interrupt enable */
#define MSTATUS_MPIE           (1 << 7) /* Machine prior interrupt enable */
#define MSTATUS_SPP            (1 << 8) /* Supervisor previous privilege */
#define MSTATUS_MPP           (3 << 11) /* Machine previous privilege */
#define MSTATUS_FS            (3 << 13) /* Floating point status */
#define MSTATUS_XS            (3 << 15) /* Extension status */
#define MSTATUS_MPRV          (1 << 17) /* Modify privilege */
#define MSTATUS_SUM           (1 << 18) /* Supervisor user memory */
#define MSTATUS_MXR           (1 << 19) /* Make executable readable */
#define MSTATUS_TVM           (1 << 20) /* Trap virtual memory */
#define MSTATUS_TW            (1 << 21) /* Timeout wait */
#define MSTATUS_TSR           (1 << 22) /* Trap sret */
#define MSTATUS_SD    (1 << (XLEN - 1)) /* Summary dirty */

#if (XLEN != 32) /* RV64 and RV128 */
#define MSTATUS_UXL           (3 << 32) /* User XLEN */
#define MSTATUS_SXL           (3 << 34) /* Supervisor XLEN */
#endif

/*
 * Mcause fields.
 */
#define MCAUSE_INT (1 << (XLEN - 1))

/*
 * Interrupt codes
 */
#define MCAUSE_IC_USI  (0) /* User software interrupt */
#define MCAUSE_IC_SSI  (1) /* Supervisor software interrupt */
#define MCAUSE_IC_MSI  (3) /* Machine software interrupt */
#define MCAUSE_IC_UTI  (4) /* User timer interrupt */
#define MCAUSE_IC_STI  (5) /* Supervisor timer interrupt */
#define MCAUSE_IC_MTI  (7) /* Machine timer interrupt */
#define MCAUSE_IC_UEI  (8) /* User external interrupt */
#define MCAUSE_IC_SEI  (9) /* Supervisor external interrupt */
#define MCAUSE_IC_MEI (11) /* Machine external interrupt */

/*
 * Exception codes
 */
#define MCAUSE_EC_IAM  (0) /* Instruction address misaligned */
#define MCAUSE_EC_IAF  (1) /* Instruction address fault */
#define MCAUSE_EC_II   (2) /* Illegal instruction */
#define MCAUSE_EC_B    (3) /* Breakpoint */
#define MCAUSE_EC_LAM  (4) /* Load address misaligned */
#define MCAUSE_EC_LAF  (5) /* Load access fault */
#define MCAUSE_EC_SAM  (6) /* Store address misaligned */
#define MCAUSE_EC_SAF  (7) /* Store access fault */
#define MCAUSE_EC_UEC  (8) /* Environment call from U-mode */
#define MCAUSE_EC_SEC  (9) /* Environment call from S-mode */
#define MCAUSE_EC_MEC (11) /* Environment call from M-mode */
#define MCAUSE_EC_IPF (12) /* Instruction page fault */
#define MCAUSE_EC_LPF (13) /* Load page fault */
#define MCAUSE_EC_SPF (15) /* Store page fault */

/*
 * Mip bits. Machine interrupt pending.
 */
#define MIP_USIP      (0) /* User software interrupt, R/W */
#define MIP_SSIP      (1) /* Supervisor software interrupt, R/W */
#define MIP_MSIP      (3) /* Machine software interrupt */
#define MIP_UTIP      (4) /* User timer interrupt, R/W */
#define MIP_STIP      (5) /* Supervisor timer interrupt, R/W */
#define MIP_MTIP      (7) /* Machine timer interrupt */
#define MIP_UEIP      (8) /* User external interrupt, R/W */
#define MIP_SEIP      (9) /* Supervisor external interrupt, R/W */
#define MIP_MEIP     (11) /* Machine external interrupt */

/*
 * Mie bits. Machine interrupt enable.
 */
#define MIE_USIE      (0) /* User software interrupt */
#define MIE_SSIE      (1) /* Supervisor software interrupt */
#define MIE_MSIE      (3) /* Machine software interrupt */
#define MIE_UTIE      (4) /* User timer interrupt */
#define MIE_STIE      (5) /* Supervisor timer interrupt */
#define MIE_MTIE      (7) /* Machine timer interrupt */
#define MIE_UEIE      (8) /* User external interrupt */
#define MIE_SEIE      (9) /* Supervisor external interrupt */
#define MIE_MEIE     (11) /* Machine external interrupt */

#ifndef __ASSEMBLER__

#include <stdint.h>

/*
 * Macros to access CSRs.
 */
#define read_csr(reg)                                   \
({                                                      \
	uintptr_t __tmp;                                \
	asm volatile ("csrr %0, " #reg : "=r"(__tmp));  \
	__tmp;                                          \
})

#define write_csr(reg, val)                             \
({                                                      \
    asm volatile ("csrw " #reg ", %0" :: "r"(val));     \
})

#define swap_csr(reg, val)                              \
({                                                      \
	uintptr_t __tmp;                                \
	asm volatile ("csrrw %0, " #reg ", %1"          \
		: "=r"(__tmp)                           \
		: "r"(val));                            \
	__tmp;                                          \
})

#define set_csr(reg, bit)                               \
({                                                      \
	uintptr_t __tmp;                                \
	asm volatile ("csrrs %0, " #reg ", %1"          \
		: "=r"(__tmp)                           \
		: "r"(bit));                            \
	__tmp;                                          \
})

#define clear_csr(reg, bit)                             \
({                                                      \
	uintptr_t __tmp;                                \
	asm volatile ("csrrc %0, " #reg ", %1"          \
		: "=r"(__tmp)                           \
		: "r"(bit));                            \
	__tmp;                                          \
})

/*
 * Memory mapped control of timer and software interrupts.
 * Typically, software interrupts are used to send signals from a core to
 * another.
 */
struct _clint {
	uint32_t msip;
	uint32_t rsv0[4095];
	uint64_t mtimecmp;
	uint32_t rsv1[8188];
	uint64_t mtime;
};
#define CLINT ((volatile struct _clint *)0x2000000)

/*
 * PLIC controls external interrupts coming from soc peripherals.
 */
struct _plic {
	uint32_t priority[52];
	uint32_t rsv0[972];
	uint32_t pending[2];
	uint32_t rsv1[1022];
	uint32_t enable[2];
	uint32_t rsv2[522238];
	uint32_t threshold;
	uint32_t claim;
};
#define PLIC ((volatile struct _plic *)0xc000000)

#endif /* __ASSEMBLER__ */

#endif /* RISCV_H_ */
