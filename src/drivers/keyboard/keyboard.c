#include "keyboard.h"
#include "types.h"
#include "vga.h"

static bool_t   shift_pressed = FALSE;

char    keyboard_getchar(void) {
	u8_t    sc;
	char    c;

	for (;;) {
		sc = keyboard_read_scancode();      /* waits for a key press */

        /* Break code (key released): Shift key, continue waiting */
		if (sc & 0x80) {
			sc &= 0x7F;
			if (sc == 0x2A || sc == 0x36)
				shift_pressed = FALSE;
			continue ;
		}

        /* Shift key held down */
		if (sc == 0x2A || sc == 0x36) {
			shift_pressed = TRUE;
			continue ;
		}

		/* F1/F2/F3 -> switch TTY, keep waiting */
		if (sc == 0x3B) { tty_switch(0); continue; }
		if (sc == 0x3C) { tty_switch(1); continue; }
		if (sc == 0x3D) { tty_switch(2); continue; }

		c = scancode_to_ascii(sc, shift_pressed);
		if (c)
			return (c);                     /* valid key -> return it */
        /* Unmapped key -> continue */
	}
}
