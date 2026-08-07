#include "keyboard.h"
#include "vga.h"
#include "shell.h"

size_t  read_line(char *buffer, size_t size) {
	size_t len = 0;

	while (1) {
		char c = keyboard_getchar();

		if (c == '\n') {
			terminal_putchar('\n');
			buffer[len] = '\0';
			return len;
		} else if (c == '\b') {
			if (len > 0) {
				len--;
				terminal_putchar('\b');
			}
		} else {
			if (len + 1 < size) {
				buffer[len++] = c;
				terminal_putchar(c);
			}
		}
	}
}
