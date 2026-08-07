#include "io.h"
#include "keyboard.h"

bool_t  poll_keyboard(void) {
	/* bit 0 of status port 0x64 = Output Buffer Full */
	return (inb(0x64) & 0x1);
}

u8_t    keyboard_read_scancode(void) {
	while (!poll_keyboard())
		;
	return (inb(0x60));
}
