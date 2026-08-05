#include "cursor.h"
#include "io.h"
#include "vga.h"

u16_t   get_cursor_pos(void) {
	u16_t   pos = 0;

	outb(0x3D4, 0xF);
	
	pos |= inb(0x3D5);
	
	outb(0x3D4, 0xE);
	
	pos |= (u16_t)(inb(0x3D5)) << 8;

	return (pos);
}

void    enable_cursor(u8_t start, u8_t end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
}

void    disable_cursor(void) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void    update_cursor(size_t x, size_t y) {
	u16_t   pos = (u16_t)(y * VGA_WIDTH + x);

	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8_t)((pos >> 8) & 0xFF));
}

