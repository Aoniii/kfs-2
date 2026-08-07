# ============================================================================
#  KFS-2 - top-level Makefile
#
#  This file stays intentionally small: it only wires the pieces together and
#  declares the high-level targets. The real logic is split across:
#
#    mk/config.mk        toolchain, paths, compiler/linker flags
#    mk/rules.mk         how to build objects, the kernel and the ISO
#    mk/toolchain.mk     sanity check that the cross-compiler exists
#    src/**/module.mk    the source list of each module (SRCS += ...)
#
#  No wildcards: every source is listed explicitly in its module.mk.
# ============================================================================

# 1. Config first - defines SRC_DIR, flags, and initializes SRCS.
include mk/config.mk

# 2. Source fragments - one per module. Each appends to SRCS.
#    Add a line here when you create a new module.
include $(SRC_DIR)/boot/module.mk
include $(SRC_DIR)/debug/module.mk
include $(SRC_DIR)/drivers/vga/module.mk
include $(SRC_DIR)/drivers/cursor/module.mk
include $(SRC_DIR)/drivers/keyboard/module.mk
include $(SRC_DIR)/gdt/module.mk
include $(SRC_DIR)/kernel/module.mk
include $(SRC_DIR)/lib/module.mk
include $(SRC_DIR)/shell/module.mk

# 3. Rules + toolchain check (need SRCS fully populated first).
include mk/rules.mk
include mk/toolchain.mk

.PHONY: all kernel iso run clean fclean re

# Default goal: build the bootable ISO.
all: iso

# Build only the kernel ELF binary.
kernel: check-toolchain $(KERNEL)

# Build the GRUB-bootable ISO image.
iso: check-toolchain $(ISO)

# Boot the ISO in an emulator.
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# Remove build objects and the ISO staging tree.
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)

# clean + remove the final artifacts (kernel + ISO).
fclean: clean
	rm -f $(KERNEL) $(ISO)

# Full rebuild.
re: fclean all
