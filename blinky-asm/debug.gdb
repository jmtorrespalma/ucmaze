# Startup script to run a gdb session
# The port is setup for openocd. For qemu, port is 1234.

# TODO: This still need to be tweaked a bit
file blinky-asm.elf
target remote localhost:4444
