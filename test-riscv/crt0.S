/*
 * Tasks to complete:
 * - Initialize stack.
 * - Copy data section to RAM
 * - Initialize bss
 * - Call platform initialization (clock, memory remap)
 * - Jump to main.
 */
	.section .init, "ax"
	.global _start
	.func   _start
_start:
.option push
.option norelax
	la	gp, _global_pointer
.option pop
	la	sp, _sp_top

	# Copy data to RAM
	la	a0, _data_rom
	la	a1, _data_start
	la	a2, _data_end
0:
	bgt	a1, a2, _clear_bss
	lw	a3, (a0)
	sw	a3, (a1)
	addi	a0, a0, 4
	addi	a1, a1, 4
	j	0b

_clear_bss:
	la	a0, _bss_start
	la	a1, _bss_end
1:
	bgt	a0, a1, 2f
	sw	x0, (a0)
	addi	a0, a0, 4
	j	1b

2:
	jal	main
	j	.

        .size   _start, . - _start
        .endfunc
