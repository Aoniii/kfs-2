#include "kernel.h"

void kernel_main(void) {
	terminal_init();
	enable_cursor(0, 14);

	putstr("42\n");
	putstr("F1/F2/F3 = TTY 0/1/2\n");

	for (;;)
		keyboard_handle();
}
