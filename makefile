# Makefile
# dd if=/dev/zero of=floppy.img bs=512 count=2880

RAMDISK		= 		floppy.img
ASM			= 		nasm
ASM_FLAGS	= 		-I include/ -f elf
CC			= 		gcc
CC_FLAGS	= 		-m32 -c -I include/
LD			= 		ld
LD_FLAGS	= 		-s -Ttext 0x30000

KERNEL		=		kernel/kernel.bin
OBJS		=		kernel/kernel.o kernel/entry.o lib/console.o lib/common.o kernel/gdt.o kernel/gdt_s.o kernel/idt.o kernel/idt_s.o kernel/i8259.o

$(RAMDISK) : boot/boot.bin boot/setup.bin kernel/kernel.bin
	dd if=boot/boot.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=boot/setup.bin of=$@ bs=512 seek=1 count=4 conv=notrunc
	dd if=kernel/kernel.bin of=$@ bs=512 seek=5 conv=notrunc

boot/boot.bin : boot/boot.s
	$(ASM) -o $@ $<

boot/setup.bin : boot/setup.s
	$(ASM) -o $@ $<

$(KERNEL) : $(OBJS)
	$(LD) $(LD_FLAGS) $(OBJS) -o $(KERNEL)

kernel/kernel.o : kernel/kernel.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

kernel/entry.o : kernel/entry.c include/console.h
	$(CC) $(CC_FLAGS) -o $@ $<

kernel/gdt_s.o : kernel/gdt_s.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

kernel/gdt.o : kernel/gdt.c include/gdt.h
	$(CC) $(CC_FLAGS) -o $@ $<

lib/console.o : lib/console.c include/types.h include/common.h
	$(CC) $(CC_FLAGS) -o $@ $<

lib/common.o : lib/common.c include/common.h
	$(CC) $(CC_FLAGS) -o $@ $<

kernel/i8259.o : kernel/i8259.c include/common.h include/types.h
	$(CC) $(CC_FLAGS) -o $@ $<

kernel/idt.o : kernel/idt.c include/idt.h include/types.h include/console.h
	$(CC) $(CC_FLAGS) -o $@ $<

kernel/idt_s.o : kernel/idt_s.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

clean :
	rm -f $(OBJS)

qemu:
	qemu -fda floppy.img -boot a

debug:
	qemu -s -S -fda floppy.img -boot a &
	sleep 1
	gdb
