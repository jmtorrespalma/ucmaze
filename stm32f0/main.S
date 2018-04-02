# Initially all pins are floating input, so for our blinky example we need to
# set them as outputs.
# Leds are in pc8 and pc9

#define RCC_BASE   0x40021000
#define RCC_AHBENR 0x14
#define IOPCEN     19
#define GPIO_BASE  0x48000800
#define GPIO_ODR   0x14
#define PIN        8
#define DELAY      0x100000

	.syntax unified
	.thumb

	.section .isr_vect
	.word _sp_init
	.word _start

	.section .init, "ax"
	.global _start
	.func   _start
	.thumb_func
_start:
	# Devices are off, so we need to enable the clock
	ldr	r1, =RCC_BASE
	movs	r2, #1
	lsls	r2, r2, #IOPCEN
	str	r2, [r1, #RCC_AHBENR]

	# Enable the pin as output
	ldr	r1, =GPIO_BASE
	movs	r2, #1
	lsls	r2, r2, #(PIN << 1)
	str	r2, [r1]

turn_on:
	movs	r2, #1
	lsls	r2, r2, #(PIN)
	str	r2, [r1, #GPIO_ODR]
	ldr	r0, =DELAY
	bl	delay

turn_off:
	movs	r2, #0
	str	r2, [r1, #GPIO_ODR]
	ldr	r0, =DELAY
	bl	delay
        b	turn_on

        .size   _start, . - _start
        .endfunc

	# Expects in r0 the parameter, time to wait.
	.func delay
delay:
	subs	r0, r0, #1
	bne	delay
	bx	lr

        .size  delay, . - delay
        .endfunc
