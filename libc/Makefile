
BUILD_DIR ?= ./build
SRC_C_DIR ?= ./src/c
SRC_ASM_DIR ?= ./src/asm
SRC_LD_DIR ?= ./src/ld
LINKER_PATH = src/ld/linker.ld

MKDIR_P ?= mkdir -p

CFLAGS = --target=i386-pc-none-elf -ffreestanding -O2 -std=c99 -Wall -Wextra
ASFLAGS = -felf32
LDFLAGS = -nostdlib

all: build/libc.a

build/libc.a: ./build/strlen.c.o
	ar rcs $@ $^

$(BUILD_DIR)/%.c.o: $(SRC_C_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	clang $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
