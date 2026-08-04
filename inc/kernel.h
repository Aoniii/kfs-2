/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 10:38:21 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/26 19:18:51 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
#define KERNEL_H

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This tutorial needs to be compiled with a i386/i686 GNU C compiler"
#endif

/////////////////////////////////////
//
//			DEFINES
//
////////////////////////////////////

#define NULL (void*)0

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000
#define VGA_SIZE    (VGA_WIDTH * VGA_HEIGHT)
#define TTY_COUNT   3

#define PTR_SIZE (sizeof(void*))

/////////////////////////////////////
//
//			TYPEDEFS
//
////////////////////////////////////

typedef unsigned long    size_t;
typedef unsigned long    u64_t;
typedef unsigned int     u32_t;
typedef unsigned short   u16_t;
typedef unsigned char    u8_t;
typedef unsigned long    ptr_t;

typedef struct s_tty
{
	u16_t  buffer[VGA_SIZE];
	size_t row;
	size_t col;
	u8_t   color;
} tty_t;

/////////////////////////////////////
//
//			IO
//
////////////////////////////////////

static inline void outb(u16_t port, u8_t val)
{
	__asm__ volatile ("out %1, %0" : : "a"(val), "Nd"(port));
}

static inline u8_t inb(u16_t port)
{
	u8_t ret = 0;

	__asm__ volatile ("in %0, %1" : "=a"(ret) : "Nd"(port));
	
	return (ret);
}

/////////////////////////////////////
//
//			ENUM
//
////////////////////////////////////

typedef enum e_bool_t
{
	FALSE,
	TRUE
} bool_t;

typedef enum e_vga_color 
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_t;

/////////////////////////////////////
//
//			TERMINAL / TTY
//
////////////////////////////////////

void   terminal_init(void);
void   terminal_set_color(u8_t color);
void   terminal_putchar(char c);
void   terminal_scroll(void);
void   terminal_writestring(const char *str);
void   tty_switch(size_t n);
size_t tty_get_current(void);

/////////////////////////////////////
//
//			UTILS
//
////////////////////////////////////

size_t ft_strlen(const char *str);
int    ft_strcmp(const char *s1, const char *s2);
int    ft_printk(const char *format, ...);
void   ft_putstr(const char *str);
void   ft_putchar(const char c);
void   ft_putnbr(int nbr);
void   ft_puthex(const u32_t n, bool_t lower);
void   ft_putptr(const ptr_t p);

/////////////////////////////////////
//
//			CURSOR
//
////////////////////////////////////

u16_t get_cursor_pos(void);
void  enable_cursor(u8_t start, u8_t end);
void  disable_cursor(void);
void  update_cursor(size_t x, size_t y);

/////////////////////////////////////
//
//			KEYBOARD
//
////////////////////////////////////

bool_t poll_keyboard(void);
u8_t   keyboard_read_scancode(void);
char   scancode_to_ascii(u8_t sc);
void   keyboard_handle(void);

/////////////////////////////////////
//
//			KERNEL
//
////////////////////////////////////

void kernel_main(void);

#endif
