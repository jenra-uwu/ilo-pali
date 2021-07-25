CODE=src/
CC=riscv64-unknown-elf-gcc
CFLAGS=-march=rv64gc -mabi=lp64d -static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles -Tkernel.ld -g -Wall -Wextra
EMU=qemu-system-riscv64
EFLAGS=-machine virt -cpu rv64 -bios opensbi-riscv64-generic-fw_dynamic.bin -m 256m -nographic -global virtio-mmio.force-legacy=false -s #-S

.PHONY: all clean run

all: $(CODE)*.s $(CODE)*.c
	$(CC) $(CFLAGS) $? -o kernel
	  
run:
	$(EMU) $(EFLAGS) -kernel kernel
