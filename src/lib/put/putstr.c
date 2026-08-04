#include "put.h"
#include "vga.h"

void    putstr(const char *s) {
	if (!s)
		return ;

	terminal_writestring(s);
}
