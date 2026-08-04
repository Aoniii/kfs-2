#include "put.h"
#include "vga.h"

void    putnbr(int n) {
	unsigned int    nb;

	if (n < 0) {
		terminal_putchar('-');
		nb = -(unsigned int)n;
	} else
        nb = (unsigned int)n;

	if (nb >= 10)
        putnbr(nb / 10);

	terminal_putchar(nb % 10 + '0');
}
