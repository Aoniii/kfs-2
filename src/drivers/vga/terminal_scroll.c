#include "vga_internal.h"

void terminal_scroll(void) {
	tty_t   *t = tty_current();
	size_t  row;
	size_t  col;
	size_t  i;

	/* Shift every line up by one. */
	for (row = 0; row < VGA_HEIGHT - 1; row++) {
		for (col = 0; col < VGA_WIDTH; col++) {
			i = row * VGA_WIDTH + col;
			t->buffer[i] = t->buffer[i + VGA_WIDTH];
			vga_mem[i] = t->buffer[i];
		}
	}

	/* Clear the last line. */
	for (col = 0; col < VGA_WIDTH; col++) {
		i = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
		t->buffer[i] = vga_entry(' ', t->color);
		vga_mem[i] = t->buffer[i];
	}
}
