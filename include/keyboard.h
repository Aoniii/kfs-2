/* PS/2 keyboard (polling). */
#ifndef KFS_KEYBOARD_H
#define KFS_KEYBOARD_H

#include "types.h"

bool_t  poll_keyboard(void);
u8_t    keyboard_read_scancode(void);
char    scancode_to_ascii(u8_t sc, bool_t shift_pressed);
char    keyboard_getchar(void);

#endif /* KFS_KEYBOARD_H */
