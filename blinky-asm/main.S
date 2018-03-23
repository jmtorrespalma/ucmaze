# This program simply starts, sets a GPIO as output and sets its value to 1 so
# we can see a let turining on.
# Selected pin is yellow led, PPA17 or AD0

# Everything runs from ROM, so no relocation is required, we can just jump
# around
# No stacks or interrupts, so we write the program directly

# 1.- Set pin as output
# 2.- Set pin value is not required because the led has a pull down resistor
# 3.- Loop forever

# Each PIO manages up to 32 pins.
# There are pull up resistors so this might be an issue.
# Writing to PIO_OER (offset = 0x10) sets pin as output.
# PIO_SODR (offset = 0x30) sets the actual value of the pin.

#define PIO       0xfffff400
#define PIO_OER   0x10
#define PIO_SODR  0x30
#define PIO_CODR  0x34
#define PIN       0x00020000
#define DELAY     0x00001000

	.arm
	.section .init, "ax"
	.global _start
	.func   _start

	# Enable the pin as output
_start:
	ldr	r0, =PIO
	mov	r1, #PIN
	str	r1, [r0, #PIO_OER]

turn_on:
	mov	r3, #DELAY
	str	r1, [r0, #PIO_CODR]
0:
	subs	r3, r3, #1
	bne	0b

turn_off:
	mov	r3, #DELAY
	str	r1, [r0, #PIO_SODR]
1:
	subs	r3, r3, #1
	bne	1b
        b	turn_on

        .size   _start, . - _start
        .endfunc
