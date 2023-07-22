
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

debug: CFLAGS += -g

# Find all source files
SRCS_C := $(shell find $(SRC_C_DIR) -name '*.c')
SRCS_ASM := $(shell find $(SRC_ASM_DIR) -name '*.asm')

# Generate object file names
OBJS_C := $(SRCS_C:$(SRC_C_DIR)/%.c=$(BUILD_DIR)/%.c.o)
OBJS_ASM := $(SRCS_ASM:$(SRC_ASM_DIR)/%.asm=$(BUILD_DIR)/%.asm.o)

all: build/os.bin

debug: build/os.bin

build/os.bin: $(OBJS_C) $(OBJS_ASM)
	clang -T $(LINKER_PATH) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.asm.o: $(SRC_ASM_DIR)/%.asm
	$(MKDIR_P) $(dir $@)
	nasm $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.c.o: $(SRC_C_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	clang $(CFLAGS) -g -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
