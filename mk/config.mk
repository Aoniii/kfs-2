# ============================================================================
#  mk/config.mk - toolchain, paths and build flags.
#  Edit this file to tune WHAT and HOW things are built.
# ============================================================================

# --- Cross-compiler (built by scripts/build-i686-elf.sh) --------------------
PREFIX      ?= $(HOME)/opt/cross
export PATH := $(PREFIX)/bin:$(PATH)

TARGET      := i686-elf
CC          := $(TARGET)-gcc
AS          := $(TARGET)-gcc
LD          := $(TARGET)-gcc

# --- Output names -----------------------------------------------------------
NAME        := kfs
KERNEL      := $(NAME).bin
ISO         := $(NAME).iso

# --- Directories ------------------------------------------------------------
SRC_DIR     := src
INC_DIR     := include
BUILD_DIR   := build
ISO_DIR     := isodir

# --- Boot / GRUB ------------------------------------------------------------
LINKER      := $(SRC_DIR)/boot/linker.ld
GRUB_CFG    := grub/grub.cfg
GRUB_FLAGS  := --compress=xz --install-modules="multiboot normal" \
               --fonts="" --locales="" --themes=""

# --- Sources ----------------------------------------------------------------
# Left empty here; each src/**/module.mk appends to it with `SRCS += ...`.
SRCS        :=

# --- Assembler flags (boot.s, GAS/intel syntax via gcc) ---------------------
ASFLAGS     := -nostartfiles -nostdlib -Os

# --- C flags — freestanding kernel ------------------------------------------
#   -ffreestanding / -nostdlib / -nodefaultlibs : no host libc
#   -fno-builtin / -fno-stack-protector         : sujet 42 flags
#   -masm=intel                                 : match our inline asm style
#   -MMD -MP                                    : auto header dependencies (.d)
CFLAGS      := -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
               -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs \
               -masm=intel -MMD -MP -I $(INC_DIR)

# --- Link flags -------------------------------------------------------------
#   -T $(LINKER) : our custom linker script (host .ld is forbidden)
#   -lgcc        : compiler runtime helpers (e.g. 64-bit math)
LDFLAGS     := -T $(LINKER) -ffreestanding -nostdlib -nostartfiles -lgcc
