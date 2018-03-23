# The goal of this program is using only assembly, be able to print a message
# using serial port, for the integrator CP board.

# The serial controller works the following way:
# - SERIAL_BASE is where we write our bytes.
# - SERIAL_FLAG_REGISTER contains flags like full buffer.
#
# Registers:
# - r0: SERIAL_BASE
# - r1: SERIAL_BUFFER_FULL
# - r2: msg iterator
# - r3: msg char container
# - r4: content of SERIAL_FLAG_REGISTER

#define SERIAL_BASE            0x16000000
#define SERIAL_FLAG_REGISTER   0x18
#define SERIAL_BUFFER_FULL     (1 << 5)

	.arm
	.section .init, "ax"
	.global _start
	.func   _start

_start:
	# Load addresses
	mov	r0, #SERIAL_BASE
	mov	r1, #SERIAL_BUFFER_FULL
	ldr	r2, =msg

load_char:
	ldrb	r3, [r2]
	cmp	r3, #0
	beq	end
	add	r2, r2, #1

	# Loop to check status
check_status:
	ldr	r4, [r0, #SERIAL_FLAG_REGISTER]
	ands	r4, r4, r1
	bne	check_status

	# Write
	str	r3, [r0]

	# Loop forever
        b	load_char

end:
	b	.

        .size   _start, . - _start
        .endfunc

# This is constants area
	.section .rodata
msg:	.asciz "hello from qemu\n"

	# Marks the end of the file
	.end
