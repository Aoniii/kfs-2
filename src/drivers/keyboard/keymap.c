#include "keyboard.h"
#include "types.h"

/* Scan Code Set 1 - index = scancode make (0x00..0x3A) */
static const char scancode_ascii[128] = {
	0,    27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
	0,    '*', 0,   ' '
};

static const char scancode_ascii_shift[128] = {
	0,    27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0,    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
	0,    '*', 0,   ' '
};

char    scancode_to_ascii(u8_t sc, bool_t shift_pressed) {
	if (sc >= 128)
		return (0);
	if (shift_pressed)
		return (scancode_ascii_shift[sc]);
	return (scancode_ascii[sc]);
}

