# Startup script to run a gdb session
# The port is setup for openocd. For qemu, port is 1234.

# TODO: This still need to be tweaked a bit
target remote localhost:3333
