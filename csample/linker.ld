ENTRY(_start)
MEMORY
{
  ROM (rx)  : ORIGIN = 0x08000000, LENGTH = 0x10000 /*64K*/
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x02000 /*8K*/
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vect))
		. = ALIGN(4);
		KEEP(*(.init))
		. = ALIGN(4);
		*(.text*)
		. = ALIGN(4);
	} > ROM

	.rodata :
	{
		*(.rodata*)
		. = ALIGN(4);
	} > ROM

	.data :
	{
		_data_rom = LOADADDR(.data);
		_data_start = .;
		*(.data*)
		. = ALIGN(4);
		_data_end = .;
	} > RAM AT > ROM

	.bss :
	{
		_bss_start = .;
		*(.bss*)
		*(COMMON*)
		. = ALIGN(4);
		_bss_end = .;
	} > RAM AT > ROM

	_sp_top = ORIGIN(RAM)+LENGTH(RAM);
}
