#include "vga_internal.h"
#include "cursor.h"

void    terminal_putchar(char c) {
	tty_t   *t = tty_current();

	if (c == '\n') {
		t->col = 0;
		t->row++;
		if (t->row >= VGA_HEIGHT) {
			terminal_scroll();
			t->row = VGA_HEIGHT - 1;
		}
		update_cursor(t->col, t->row);
		return;
	}

	/* Backspace: step back and erase the previous cell. */
	if (c == '\b') {
		if (t->col == 0) {
			if (t->row == 0)
				return;
			t->row--;
			t->col = VGA_WIDTH - 1;
		} else {
			t->col--;
		}
		tty_set_cell(t->row, t->col, vga_entry(' ', t->color));
		update_cursor(t->col, t->row);
		return;
	}

	tty_set_cell(t->row, t->col, vga_entry((u8_t)c, t->color));

	if (++t->col == VGA_WIDTH) {
		t->col = 0;
		t->row++;
		if (t->row >= VGA_HEIGHT) {
			terminal_scroll();
			t->row = VGA_HEIGHT - 1;
		}
	}

	update_cursor(t->col, t->row);
}
