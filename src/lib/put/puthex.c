#include "put.h"
#include "types.h"

void    puthex(u32_t n, bool_t lower, bool_t prefix) {
	const char  *base = lower ? "0123456789abcdef" : "0123456789ABCDEF";
	int         i;

    if (prefix)
        putstr("0x");
	
    i = 28;
	while (i >= 0) {
		putchar(base[(n >> i) & 0xF]);
		i -= 4;
	}
}
