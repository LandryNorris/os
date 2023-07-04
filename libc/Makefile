
BUILD_DIR ?= ./build
SRC_C_DIR ?= ./src/c
SRC_ASM_DIR ?= ./src/asm
SRC_LD_DIR ?= ./src/ld
LINKER_PATH = src/ld/linker.ld

MKDIR_P ?= mkdir -p

INCLUDE_FLAGS = -I ./include -I../libk/include
CFLAGS = --target=i386-pc-none-elf -ffreestanding -O2 -std=c99 -Wall -Wextra $(INCLUDE_FLAGS)
ASFLAGS = -felf32
LDFLAGS = -nostdlib -L../libk/build -lk

all: build/libc.a

build/libc.a: ./build/string/strlen.c.o ./build/stdio/putc.c.o ./build/stdio/puts.c.o ./build/stdio/printf.c.o ./build/string/memutils.c.o ./build/string/strcat.c.o ./build/string/strchr.c.o ./build/string/strcpy.c.o
	ar rcs $@ $^

$(BUILD_DIR)/%.c.o: $(SRC_C_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	clang $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
