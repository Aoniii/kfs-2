# Freestanding mini-libc — one function per file.
SRCS += $(SRC_DIR)/lib/fmt/printk.c

SRCS += $(SRC_DIR)/lib/put/putchar.c
SRCS += $(SRC_DIR)/lib/put/puthex.c
SRCS += $(SRC_DIR)/lib/put/putnbr.c
SRCS += $(SRC_DIR)/lib/put/putptr.c
SRCS += $(SRC_DIR)/lib/put/putstr.c

SRCS += $(SRC_DIR)/lib/str/strcmp.c
SRCS += $(SRC_DIR)/lib/str/strlen.c
