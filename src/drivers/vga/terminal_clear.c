#include "vga_internal.h"
#include "cursor.h"

/* Wipe the current TTY: fill its buffer with blanks, reset cursor to (0,0). */
void    terminal_clear(void) {
	tty_t   *t = tty_current();
	size_t  i;

	for (i = 0; i < VGA_SIZE; i++)
		t->buffer[i] = vga_entry(' ', t->color);

	tty_flush(vga_current);

	t->row = 0;
	t->col = 0;
	update_cursor(0, 0);
}
