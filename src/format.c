/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 17:31:50 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/23 17:41:24 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/kernel.h"
#include <stdarg.h>

/////////////////////////////////////
//
//			PRINTK
//
////////////////////////////////////

int ft_printk(const char *format, ...)
{
	va_list list;
	void *ptr = NULL;
	u32_t x = 0;
	ptr_t p = 0;
	int nb = 0;
	char c = 0;

	va_start(list, format);

	while (*format) {

		while (*format && *format != '%')
			terminal_putchar(*format++);

		if (!*format)
			break;

		format++;
		
		switch (*format) {
			case 'd':
				nb = (int)va_arg(list, int);
				ft_putnbr(nb);
				format++;
				break;

			case 's':
				ptr = (char *)va_arg(list, char *);
				ft_putstr((char *)ptr);
				format++;
				break;

			case 'x':
				x = (u32_t)va_arg(list, u32_t);
				ft_puthex(x, TRUE);
				format++;
				break;

			case 'c':
				c = (char)va_arg(list, int);
				ft_putchar(c);
				format++;
				break;

			case 'p':
				p = va_arg(list, ptr_t);
				ft_putptr(p);
				format++;
				break;

			default:
				terminal_putchar(*format++);
				break;
		}
	}

	va_end(list);
	
	return (0);
}
