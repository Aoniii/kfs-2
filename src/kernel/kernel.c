#include "kernel.h"
#include "shell.h"

void kernel_main(void) {
    gdt_init();
	terminal_init();
	enable_cursor(0, 14);
    shell_run();
}
