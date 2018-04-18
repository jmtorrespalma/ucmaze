
	.syntax unified
	.thumb

	.section .isr_vect
	.word _sp_top
	.word _start
	.word _startup_handler
	.word _startup_handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word _startup_handler
	.word 0
	.word 0
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word 0
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word _startup_handler
	.word 0
	.word _startup_handler


/*
 * - Copy data section to RAM
 * - Initialize bss
 * - Call platform initialization (clock, memory remap)
 * - Jump to main.
 */
	.section .init, "ax"
	.global _start
	.func   _start
	.thumb_func
_start:
	# Copy data to RAM
	ldr	r0, =_data_rom
	ldr	r1, =_data_start
	ldr	r2, =_data_end
0:
	cmp	r1, r2
	beq	_clear_bss
	ldr	r3, [r0]
	str	r3, [r1]
	adds	r0, r0, #4
	adds	r1, r1, #4
	b	0b

_clear_bss:
	ldr	r0, =_bss_start
	ldr	r1, =_bss_end
	movs	r2, #0
1:
	cmp	r0, r1
	beq	2f
	str	r2, [r0]
	adds	r0, r0, #4
	b	1b

2:
	bl	main
	b	.

        .size   _start, . - _start
        .endfunc

/*
 * This function is called if an interrupt occurs in early boot, which is
 * nothing but a disaster.
 */
	.global _startup_handler
	.func   _startup_handler
	.thumb_func
        .size   _startup_handler, . - _startup_handler
_startup_handler:
	b	_startup_handler
        .endfunc
