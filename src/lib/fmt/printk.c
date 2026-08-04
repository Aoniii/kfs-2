#include "vga.h"
#include "put.h"
#include <stdarg.h>

int printk(const char *format, ...) {
	va_list list;
	void    *ptr = NULL;
	u32_t   x = 0;
	ptr_t   p = 0;
	int     nb = 0;
	char    c = 0;

	va_start(list, format);

	while (*format) {
		while (*format && *format != '%')
			terminal_putchar(*format++);

		if (!*format)
			break;

		format++;
		switch (*format) {
			case 'd':
				nb = (int)va_arg(list, int);
				putnbr(nb);
				format++;
				break;

			case 's':
				ptr = (char *)va_arg(list, char *);
				putstr((char *)ptr);
				format++;
				break;

			case 'x':
				x = (u32_t)va_arg(list, u32_t);
				puthex(x, TRUE);
				format++;
				break;

			case 'c':
				c = (char)va_arg(list, int);
				putchar(c);
				format++;
				break;

			case 'p':
				p = va_arg(list, ptr_t);
				putptr(p);
				format++;
				break;

			default:
				terminal_putchar(*format++);
				break;
		}
	}

	va_end(list);
	return (0);
}
