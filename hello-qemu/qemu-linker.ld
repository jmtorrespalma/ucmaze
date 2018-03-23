/*
 * The qemu emulator puts its own instructions on 0x0, the reset vector,
 * and then jumps to 0x10000, so we must place our program there.
 */
ENTRY(_start)
SECTIONS
{
	. = 0x10000;
	.text : {
		*(.init*)
		*(.text*)
		*(.rodata*)
	}

	.data : {
		*(.data*)
	}

	.bss : {
		*(.bss*)
		*(COMMON*)
	}
}
