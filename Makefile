TARGET = myos

CC = i686-elf-gcc
LD = i686-elf-gcc

CFLAGS = -std=gnu99 \
         -ffreestanding \
         -O2 \
         -Wall \
         -Wextra

LDFLAGS = -T linker.ld \
          -ffreestanding \
          -O2 \
          -nostdlib

OBJS = boot.o kernel.o

all: $(TARGET).iso

boot.o: boot.s
	$(CC) -c boot.s -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS) -lgcc

$(TARGET).iso: kernel.bin grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET).iso iso

run: $(TARGET).iso
	qemu-system-i386 -cdrom $(TARGET).iso

clean:
	rm -rf *.o *.bin *.iso iso

.PHONY: all run clean
