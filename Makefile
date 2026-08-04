# KFS-01 Makefile — i686-elf kernel + ISO

PREFIX		?= $(HOME)/opt/cross
export PATH	:= $(PREFIX)/bin:$(PATH)

TARGET		:= i686-elf
CC			:= $(TARGET)-gcc
AS			:= $(TARGET)-gcc
LD			:= $(TARGET)-gcc

NAME		:= kfs
KERNEL		:= $(NAME).bin
ISO			:= $(NAME).iso

SRC_DIR		:= src
INC_DIR		:= inc
BUILD_DIR	:= build
ISO_DIR		:= isodir
GRUB_CFG	:= grub/grub.cfg
GRUB_FLAGS	:= --compress=xz --install-modules="multiboot normal" --fonts="" --locales="" --themes=""

LINKER		:= $(SRC_DIR)/linker.ld

ASM_SRCS	:= $(SRC_DIR)/boot.s
C_SRCS		:= $(SRC_DIR)/terminal.c $(SRC_DIR)/utils.c $(SRC_DIR)/cursor.c $(SRC_DIR)/kernel.c $(SRC_DIR)/format.c $(SRC_DIR)/keyboard.c
ASM_OBJS	:= $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SRCS))
C_OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
OBJS		:= $(ASM_OBJS) $(C_OBJS)

# Bootloader / ASM (as requested)
ASFLAGS		:= -c -nostartfiles -nostdlib -Os

# Kernel C — freestanding (sujet 42 + Bare Bones)
CFLAGS		:= -std=gnu99 \
			   -ffreestanding \
			   -O2 \
			   -Wall \
			   -Wextra \
			   -fno-builtin \
			   -fno-stack-protector \
			   -nostdlib \
			   -nodefaultlibs \
			   -masm=intel \
			   -I $(INC_DIR)

# Link
LDFLAGS		:= -T $(LINKER) \
			   -ffreestanding \
			   -nostdlib \
			   -nostartfiles \
			   -lgcc

.PHONY: all kernel iso clean fclean re run check-toolchain

all: iso

kernel: check-toolchain $(KERNEL)

iso: $(ISO)

check-toolchain:
	@command -v $(CC) >/dev/null 2>&1 || { \
		echo "error: $(CC) not found. Add $(PREFIX)/bin to PATH or build the toolchain:"; \
		echo "  ./scripts/build-i686-elf.sh"; \
		exit 1; \
	}

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(ISO): $(KERNEL) $(GRUB_CFG)
	@command -v grub-mkrescue >/dev/null 2>&1 || { \
		echo "error: grub-mkrescue not found (install grub-pc-bin / grub2-common)"; \
		exit 1; \
	}
	@command -v xorriso >/dev/null 2>&1 || { \
		echo "error: xorriso not found — install it with:"; \
		echo "  sudo apt install xorriso"; \
		exit 1; \
	}
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/$(KERNEL)
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR) $(GRUB_FLAGS)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)

fclean: clean
	rm -f $(KERNEL) $(ISO)

re: fclean all
