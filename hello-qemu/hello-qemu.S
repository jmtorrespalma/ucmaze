# The goal of this program is using only assembly, be able to print a message
# using serial port, for the integrator CP board.

# The serial controller works the following way:
# - SERIAL_BASE is where we write our bytes.
# - SERIAL_FLAG_REGISTER contains flags like full buffer.
#
# Registers:
# - r0: SERIAL_BASE
# - r1: SERIAL_BUFFER_FULL
# - r2: msg pointer

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
	mov	r2, #'A'

	# Loop to check status
check_status:
	ldr	r3, [r0, #SERIAL_FLAG_REGISTER]
	ands	r3, r3, r1
	bne	check_status

	# Write
	str	r2, [r0]

	# Loop forever
        b	check_status

        .size   _start, . - _start
        .endfunc

# This is constants area
	.section .rodata
msg:	.asciz "hello from qemu"

	# Marks the end of the file
	.end
