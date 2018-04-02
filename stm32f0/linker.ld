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
	} >ROM
	_sp_init = ORIGIN(RAM)+LENGTH(RAM) - 0x20;
}
