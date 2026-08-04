/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 19:14:11 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/26 19:20:24 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/kernel.h"

/* Scan Code Set 1 — index = scancode make (0x00..0x3A) */
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

static bool_t shift_pressed = FALSE;

bool_t poll_keyboard(void)
{
	/* bit 0 of status port 0x64 = Output Buffer Full */
	return (inb(0x64) & 0x1);
}

u8_t keyboard_read_scancode(void)
{
	while (!poll_keyboard())
		;
	return (inb(0x60));
}

char scancode_to_ascii(u8_t sc)
{
	if (sc >= 128)
		return (0);
	if (shift_pressed)
		return (scancode_ascii_shift[sc]);
	return (scancode_ascii[sc]);
}

void keyboard_handle(void)
{
	u8_t sc = 0;
	char c = 0;

	if (!poll_keyboard())
		return;

	sc = inb(0x60);

	/* Break code (touche relâchée) : bit 7 à 1 */
	if (sc & 0x80) {
		sc &= 0x7F;
		if (sc == 0x2A || sc == 0x36)
			shift_pressed = FALSE;
		return;
	}

	/* Make codes Shift gauche / droite */
	if (sc == 0x2A || sc == 0x36) {
		shift_pressed = TRUE;
		return;
	}

	/* F1 / F2 / F3 → switch TTY 0 / 1 / 2 */
	if (sc == 0x3B) {
		tty_switch(0);
		return;
	}
	if (sc == 0x3C) {
		tty_switch(1);
		return;
	}
	if (sc == 0x3D) {
		tty_switch(2);
		return;
	}

	c = scancode_to_ascii(sc);
	if (c)
		terminal_putchar(c);
}
