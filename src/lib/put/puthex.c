#include "put.h"

void    puthex(u32_t n, bool_t lower) {
	const char  *base;
	int         i;

	base = lower ? "0123456789abcdef" : "0123456789ABCDEF";
	putstr("0x");
	i = 28;
	while (i >= 0) {
		putchar(base[(n >> i) & 0xF]);
		i -= 4;
	}
}
