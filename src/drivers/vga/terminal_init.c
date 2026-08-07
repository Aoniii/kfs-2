#include "put.h"
#include "vga_internal.h"
#include "cursor.h"

void    terminal_init(void) {
	static const vga_color_t fg[TTY_COUNT] = {
		VGA_COLOR_GREEN,
		VGA_COLOR_CYAN,
		VGA_COLOR_LIGHT_MAGENTA
	};

	size_t  t;
	size_t  i;
	tty_t   *tty;

	for (t = 0; t < TTY_COUNT; t++) {
		tty = &vga_ttys[t];
		tty->row = 0;
		tty->col = 0;
		tty->color = vga_entry_color(fg[t], VGA_COLOR_BLACK);
		for (i = 0; i < VGA_SIZE; i++)
			tty->buffer[i] = vga_entry(' ', tty->color);
	}

	vga_current = 0;
	tty_flush(0);
	update_cursor(0, 0);

	/* Banner on each TTY to visualize the F1/F2/F3 switch. */
	for (t = 0; t < TTY_COUNT; t++) {
		vga_current = t;
		tty_flush(t);
        putstr("kfs>");
	}

	vga_current = 0;
	tty_flush(0);
	update_cursor(vga_ttys[0].col, vga_ttys[0].row);
}
