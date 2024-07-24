/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 09:52:20 by user4t            #+#    #+#             */
/*   Updated: 2023/09/26 13:09:08 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_take_var(va_list ap, char c)
{
	int		len;

	len = 1;
	if (c == 'c')
		len = ft_print_char(ap);
	else if (c == 's')
		len = ft_print_str(ap);
	else if (c == 'p')
		len = ft_print_pointer(ap);
	else if (c == 'd' || c == 'i' || c == 'u')
		len = ft_print_i_d_u(ap, c);
	else if (c == 'x')
		len = ft_print_hex(ap, c);
	else if (c == 'X')
		len = ft_print_hex(ap, c);
	else if (c == '%')
		len = ft_print_percent(c);
	return (len);
}

int	ft_printf(const char *s, ...)
{
	int		len;
	va_list	ap;

	len = 0;
	va_start(ap, s);
	while (*s)
	{
		if (*s == '%')
		{
			if (ft_strchr(VARDEC, *(++s)))
				len = len + ft_take_var(ap, *s);
			else
				break ;
		}
		else
		{
			ft_putchar_fd(*s, 1);
			len++;
		}
		s++;
	}
	return (len);
}
