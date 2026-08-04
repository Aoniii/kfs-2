/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam0verfl0w <stales@student.42angouleme.f  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 07:59:57 by sam0verfl0w       #+#    #+#             */
/*   Updated: 2026/07/26 19:21:18 by sam0verfl0w      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/kernel.h"

/////////////////////////////////////
/*			KERNEL ENTRY           */
/////////////////////////////////////

void kernel_main(void)
{
	terminal_init();
	enable_cursor(0, 14); /* underscore visible */

	//int a = 32;

	ft_putstr("42\n");
	ft_putstr("F1/F2/F3 = TTY 0/1/2\n");
	/* curseur clignote juste apres le texte — ne pas appeler disable_cursor */

	for (;;)
		keyboard_handle();
}
