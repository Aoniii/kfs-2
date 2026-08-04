#include "vga_internal.h"
#include "cursor.h"

void    tty_switch(size_t n) {
	tty_t   *t;

	if (n >= TTY_COUNT || n == vga_current)
		return;

	vga_current = n;
	t = tty_current();
	tty_flush(n);
	update_cursor(t->col, t->row);
}

size_t  tty_get_current(void) {
	return (vga_current);
}
