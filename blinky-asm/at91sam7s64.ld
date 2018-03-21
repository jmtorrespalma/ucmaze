MEMORY
{
  ROM (rx)  : ORIGIN = 0x00100000, LENGTH = 0x00010000
  RAM (rwx) : ORIGIN = 0x00200000, LENGTH = 0x00004000
}

SECTIONS
{
	.text :
	{
		KEEP(*(.init))
		*(.text .text.*)                   /* remaining code */
		. = ALIGN(4);
	} >ROM
}
