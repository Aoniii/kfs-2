/* VGA text-mode terminal and virtual TTYs. */
#ifndef KFS_VGA_H
#define KFS_VGA_H

#include "types.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000
#define VGA_SIZE    (VGA_WIDTH * VGA_HEIGHT)
#define TTY_COUNT   3

typedef enum
{
	VGA_COLOR_BLACK         = 0,
	VGA_COLOR_BLUE          = 1,
	VGA_COLOR_GREEN         = 2,
	VGA_COLOR_CYAN          = 3,
	VGA_COLOR_RED           = 4,
	VGA_COLOR_MAGENTA       = 5,
	VGA_COLOR_BROWN         = 6,
	VGA_COLOR_LIGHT_GREY    = 7,
	VGA_COLOR_DARK_GREY     = 8,
	VGA_COLOR_LIGHT_BLUE    = 9,
	VGA_COLOR_LIGHT_GREEN   = 10,
	VGA_COLOR_LIGHT_CYAN    = 11,
	VGA_COLOR_LIGHT_RED     = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN   = 14,
	VGA_COLOR_WHITE         = 15,
}   vga_color_t;

typedef struct
{
	u16_t  buffer[VGA_SIZE];
	size_t row;
	size_t col;
	u8_t   color;
}   tty_t;

void    terminal_init(void);
void    terminal_set_color(u8_t color);
void    terminal_putchar(char c);
void    terminal_write(const char *data, size_t size);
void    terminal_writestring(const char *str);
void    terminal_scroll(void);
void    terminal_clear(void);

void    tty_switch(size_t n);
size_t  tty_get_current(void);

#endif /* KFS_VGA_H */
