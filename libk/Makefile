
BUILD_DIR ?= ./build
SRC_C_DIR ?= ./src/c
SRC_ASM_DIR ?= ./src/asm
SRC_LD_DIR ?= ./src/ld
LINKER_PATH = src/ld/linker.ld

MKDIR_P ?= mkdir -p

INCLUDE_FLAGS = -I./include
CFLAGS = --target=i386-pc-none-elf -ffreestanding -std=c99 -Wall -Wextra $(INCLUDE_FLAGS)
ASFLAGS = -felf32
LDFLAGS = -nostdlib

debug: CFLAGS += -g

# Find all source files
SRCS_C := $(shell find $(SRC_C_DIR) -name '*.c')

# Generate object file names
OBJS_C := $(SRCS_C:$(SRC_C_DIR)/%.c=$(BUILD_DIR)/%.c.o)

all: build/libk.a

debug: build/libk.a

build/libk.a: $(OBJS_C)
	ar rcs $@ $^

$(BUILD_DIR)/%.c.o: $(SRC_C_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	clang $(CFLAGS) -g -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
