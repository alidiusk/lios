C_SOURCES=$(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS=$(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
# nice syntax for file extension replacement
OBJ=$(C_SOURCES:.c=.o cpu/interrupt.o)

CC=clang
ASM=nasm
GDB=gdb
LD=ld.lld

TARGET=i386-linux-elf
LDFLAGS=--Ttext=0x1000
CFLAGS=-Wall -Wextra -Wpedantic -Werror -g -O2 -ffreestanding -m32 -fno-exceptions

# first rule is run by default
os_image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

#
bootsect.bin: bootsect.asm
	$(ASM) $^ -f bin -o $@

# --oformat binary strips symbols, don't have to manually do it
kernel.bin: boot/kernel_entry.o $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS) --oformat binary

# for debugging purposes
kernel.elf: boot/kernel_entry.o $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

run: os_image.bin
	qemu-system-i386 -fda $<

# open the connection to qemu and load our kernel-object file with symbols
debug: os_image.bin kernel.elf
	qemu-system-i386 -s -fda os_image.bin -d guest_errors,int &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# generic rules for wildcards
# to make an object, always compile from its .c

%.o: %.c $(HEADERS)
	$(CC) -target $(TARGET) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

.PHONY: clean

clean:
	rm -rf *.o *.bin *.elf peda*
	rm -rf kernel/*.o drivers/*.o boot/*.o boot/*.bin cpu/*.o libc/*.o
