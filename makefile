# Makefile
# dd if=/dev/zero of=floppy.img bs=512 count=2880

RAMDISK		= 		floppy.img
ASM			= 		nasm
ASMKFLAGS	= 		-I include/ -f elf
ASMBFLAGS	= 		-I boot/include/
CC			= 		cc
CFLAGS		= 		-I include/

$(RAMDISK) : boot/boot.bin boot/setup.bin kernel/kernel.bin
	dd if=boot/boot.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=boot/setup.bin of=$@ bs=512 seek=1 count=4 conv=notrunc
	dd if=kernel/kernel.bin of=$@ bs=512 seek=5 conv=notrunc

boot/boot.bin : boot/boot.s
	$(ASM) -o $@ $<

boot/setup.bin : boot/setup.s
	$(ASM) -o $@ $<

kernel/kernel.bin : kernel/kernel.s
	$(ASM) -o $@ $<

qemu:
	qemu -fda floppy.img -boot a

debug:
	qemu -s -S -fda floppy.img -boot a &
	sleep 1
	gdb
