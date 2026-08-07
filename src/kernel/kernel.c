#include "kernel.h"

void kernel_main(void) {
    gdt_init();
	terminal_init();
	enable_cursor(0, 14);

    while (1) keyboard_handle();
}
