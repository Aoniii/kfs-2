#include "vga_internal.h"
#include "str.h"

void    terminal_write(const char *data, size_t size) {
	size_t  i;

	for (i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void    terminal_writestring(const char *s) {
	terminal_write(s, strlen(s));
}
