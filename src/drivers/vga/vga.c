#include "vga_internal.h"

/* Shared VGA/TTY state (external linkage — declared in vga_internal.h). */
tty_t   vga_ttys[TTY_COUNT];
size_t  vga_current = 0;
u16_t   *vga_mem = (u16_t *)VGA_MEMORY;

/* Write one cell into the current TTY buffer and mirror it to VGA memory. */
void    tty_set_cell(size_t row, size_t col, u16_t entry) {
	size_t  idx = row * VGA_WIDTH + col;

	vga_ttys[vga_current].buffer[idx] = entry;
	vga_mem[idx] = entry;
}

/* Copy TTY n's backing buffer to the visible VGA memory. */
void    tty_flush(size_t n) {
	size_t  i;

	for (i = 0; i < VGA_SIZE; i++)
		vga_mem[i] = vga_ttys[n].buffer[i];
}

/* Set the color used for subsequent writes on the current TTY. */
void    terminal_set_color(u8_t color) {
	tty_current()->color = color;
}
