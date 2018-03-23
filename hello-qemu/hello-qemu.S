# The goal of this program is using only assembly, be able to print a message
# using serial port, for the integrator CP board.

# The serial controller works the following way:
# - SERIAL_BASE is where we write our bytes.
# - SERIAL_FLAG_REGISTER contains flags like full buffer.

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
	mov	r1, #0
	add	r1, r1, #SERIAL_FLAG_REGISTER
	ldr	r2, =msg

	# Start writing loop



	# Loop forever
        b	.

        .size   _start, . - _start
        .endfunc

# This is constants area
	.section .rodata
msg:	.asciz  "hello from qemu"

	# Marks the end of the file
	.end
