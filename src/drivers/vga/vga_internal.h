/* Private header of the VGA/TTY driver.
   NOT part of the public API — lives in the module directory, never in
   include/. Only the driver's own .c files include it. It exposes the
   shared state and helpers that the split translation units need. */
#ifndef KFS_VGA_INTERNAL_H
#define KFS_VGA_INTERNAL_H

#include "vga.h"   /* public API: tty_t, VGA_* defines, vga_color_t, protos */

/* --- Shared state (defined in vga.c) -------------------------------------
   External linkage because it is shared across the driver's files, hence
   the vga_ prefix to avoid clashing with any other global in the kernel. */
extern tty_t    vga_ttys[TTY_COUNT];
extern size_t   vga_current;
extern u16_t    *vga_mem;

/* --- Small private helpers (inline, no state of their own) ---------------- */
static inline u8_t  vga_entry_color(vga_color_t fg, vga_color_t bg) {
	return (u8_t)(fg | (bg << 4));
}

static inline u16_t vga_entry(u8_t uc, u8_t color) {
	return (u16_t)uc | (u16_t)(color << 8);
}

static inline tty_t *tty_current(void) {
	return (&vga_ttys[vga_current]);
}

/* --- Bigger private helpers (defined in vga.c) ---------------------------- */
void    tty_set_cell(size_t row, size_t col, u16_t entry);
void    tty_flush(size_t n);

#endif /* KFS_VGA_INTERNAL_H */
