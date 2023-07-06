
BUILD_DIR ?= ./build
SRC_C_DIR ?= ./src/c
SRC_ASM_DIR ?= ./src/asm
SRC_LD_DIR ?= ./src/ld
LINKER_PATH = src/ld/linker.ld

MKDIR_P ?= mkdir -p

INCLUDE_FLAGS = -I./include -I../libc/include -I../libk/include
CFLAGS = --target=i386-pc-none-elf -ffreestanding -O2 -std=c99 -Wall -Wextra $(INCLUDE_FLAGS)
ASFLAGS = -felf32
LDFLAGS = -nostdlib -L../libc/build/ -L../libk/build -lc -lk

all: build/os.bin

build/os.bin: ./build/kernel.c.o ./build/boot.asm.o ./build/gdt.asm.o ./build/gdt/gdt.c.o ./build/pic/pic.c.o ./build/exceptions.asm.o ./build/interrupts.asm.o ./build/isr/interrupt.c.o ./build/isr/exception.c.o ./build/idt/idt.c.o ./build/interrupts/keyboard.c.o ./build/idt.asm.o
	clang -T $(LINKER_PATH) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.asm.o: $(SRC_ASM_DIR)/%.asm
	$(MKDIR_P) $(dir $@)
	nasm $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.c.o: $(SRC_C_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	clang $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
