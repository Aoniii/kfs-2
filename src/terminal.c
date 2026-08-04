/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 10:44:17 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/26 19:40:00 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/kernel.h"

/////////////////////////////////////
//
//			GLOBALS
//
////////////////////////////////////

static tty_t  ttys[TTY_COUNT];
static size_t current_tty = 0;
static u16_t  *vga = (u16_t *)VGA_MEMORY;

/////////////////////////////////////
//
//			INLINES FUNCTIONS
//
////////////////////////////////////

static inline u8_t vga_entry_color(vga_color_t fg, vga_color_t bg)
{
	return (fg | bg << 4);
}

static inline u16_t vga_entry(u8_t uc, u8_t color)
{
	return ((u16_t)uc | (u16_t)(color << 8));
}

static inline tty_t *tty_current(void)
{
	return (&ttys[current_tty]);
}

/* Écrit dans le buffer du TTY courant + miroir VGA */
static void tty_set_cell(size_t row, size_t col, u16_t entry)
{
	size_t idx = row * VGA_WIDTH + col;

	ttys[current_tty].buffer[idx] = entry;
	vga[idx] = entry;
}

static void tty_flush(size_t n)
{
	size_t i;

	for (i = 0; i < VGA_SIZE; i++)
		vga[i] = ttys[n].buffer[i];
}

/////////////////////////////////////
//
//			TTY
//
////////////////////////////////////

void tty_switch(size_t n)
{
	tty_t *t;

	if (n >= TTY_COUNT || n == current_tty)
		return;

	current_tty = n;
	t = tty_current();
	tty_flush(n);
	update_cursor(t->col, t->row);
}

size_t tty_get_current(void)
{
	return (current_tty);
}

void terminal_init(void)
{
	static const vga_color_t fg[TTY_COUNT] = {
		VGA_COLOR_GREEN,
		VGA_COLOR_CYAN,
		VGA_COLOR_LIGHT_MAGENTA
	};

	size_t t;
	size_t i;
	tty_t  *tty = NULL;

	for (t = 0; t < TTY_COUNT; t++) {
		tty = &ttys[t];
		tty->row = 0;
		tty->col = 0;
		tty->color = vga_entry_color(fg[t], VGA_COLOR_BLACK);
		for (i = 0; i < VGA_SIZE; i++)
			tty->buffer[i] = vga_entry(' ', tty->color);
	}

	current_tty = 0;
	tty_flush(0);
	update_cursor(0, 0);

	/* Bannière sur chaque TTY pour visualiser le switch (F1/F2/F3) */
	for (t = 0; t < TTY_COUNT; t++) {
		current_tty = t;
		tty_flush(t);
		ft_printk("TTY %d — F1/F2/F3 pour changer d'ecran\n", (int)t);
	}

	current_tty = 0;
	tty_flush(0);
	update_cursor(ttys[0].col, ttys[0].row);
}

void terminal_set_color(u8_t color)
{
	tty_current()->color = color;
}

void terminal_putchar(char c)
{
	tty_t *t = tty_current();

	if (c == '\n') {
		t->col = 0;
		t->row++;
		if (t->row >= VGA_HEIGHT) {
			terminal_scroll();
			t->row = VGA_HEIGHT - 1;
		}
		update_cursor(t->col, t->row);
		return;
	}

	/* Backspace : reculer et effacer le caractère précédent */
	if (c == '\b') {
		if (t->col == 0) {
			if (t->row == 0)
				return;
			t->row--;
			t->col = VGA_WIDTH - 1;
		} else {
			t->col--;
		}
		tty_set_cell(t->row, t->col, vga_entry(' ', t->color));
		update_cursor(t->col, t->row);
		return;
	}

	tty_set_cell(t->row, t->col, vga_entry(c, t->color));

	if (++t->col == VGA_WIDTH) {
		t->col = 0;
		t->row++;
		if (t->row >= VGA_HEIGHT) {
			terminal_scroll();
			t->row = VGA_HEIGHT - 1;
		}
	}

	update_cursor(t->col, t->row);
}

void terminal_write(const char *data, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *s)
{
	terminal_write(s, ft_strlen(s));
}

void terminal_scroll(void)
{
	tty_t  *t = tty_current();
	size_t row;
	size_t col;
	size_t i;

	for (row = 0; row < VGA_HEIGHT - 1; row++) {
		for (col = 0; col < VGA_WIDTH; col++) {
			i = row * VGA_WIDTH + col;
			t->buffer[i] = t->buffer[i + VGA_WIDTH];
			vga[i] = t->buffer[i];
		}
	}

	for (col = 0; col < VGA_WIDTH; col++) {
		i = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
		t->buffer[i] = vga_entry(' ', t->color);
		vga[i] = t->buffer[i];
	}
}
