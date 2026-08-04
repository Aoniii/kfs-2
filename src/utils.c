/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 10:47:19 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/23 17:40:56 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/kernel.h"

/////////////////////////////////////
//
//			STRLEN
//
////////////////////////////////////

size_t ft_strlen(const char *str)
{
	if (!str)
		return (0);

	char *tmp = (char *)str;

	while (*tmp)
		tmp++;

	return (tmp - str);
}

/////////////////////////////////////
//
//			STRCMP
//
////////////////////////////////////

int ft_strcmp(const char *s1, const char *s2)
{
	u8_t *t1 = (u8_t*)s1;
	u8_t *t2 = (u8_t*)s2;

	while (*t1 && *t2 && *t1 == *t2) {
		t1++;
		t2++;
	}

	return (*t1 - *t2);
}

/////////////////////////////////////
//
//			PUTSTR
//
////////////////////////////////////

void ft_putstr(const char *s)
{
	if (!s)
		return ;

	terminal_writestring(s);
}

void ft_putchar(const char c)
{
	terminal_putchar(c);
}

void ft_putnbr(int nbr)
{
	u64_t rev = 0;

	if (nbr < 0) {
		terminal_putchar('-');
		nbr = ~(~-(nbr));
	}

	u32_t nb = (u32_t)nbr;

	while (nb) {
		rev += nb % 10;
		nb /= 10;
		rev *= 10;
	}

	rev /= 10;

	while (rev > 0) {
		terminal_putchar((char)((rev % 10) + '0'));
		rev /= 10;
	}
}

void ft_puthex(const u32_t n, bool_t lower)
{
	static const char HEX_TAB[] = "0123456789ABCDEF";

	u8_t byte = 0;

	terminal_writestring("0x");

	for (int i = 3; i >= 0; i--) {

		byte = (n >> (8 * i)) & 0xFF;
		
		terminal_putchar(lower 
			? HEX_TAB[(byte >> 4) & 0xF] | 0x20
			: HEX_TAB[(byte >> 4) & 0xF]);

		terminal_putchar(lower 
			? HEX_TAB[(byte & 0xF)] | 0x20 
			: HEX_TAB[(byte & 0xF)]);

	}
}

void ft_putptr(const ptr_t p)
{
	static const char HEX_TAB[] = "0123456789abcdef";

	u8_t byte = 0;

	terminal_writestring("0x");
	
	for (int i = PTR_SIZE - 1; i >= 0; i--) {
		
		byte = (p >> (8 * i)) & 0xFF;

		terminal_putchar(HEX_TAB[(byte >> 4) & 0xF]);
		terminal_putchar(HEX_TAB[(byte & 0xF)]);
	}
}

