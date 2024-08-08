/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 12:32:05 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:09:06 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_print_char(va_list ap)
{
	char	c;

	c = va_arg(ap, int);
	ft_putchar_fd(c, 1);
	return (1);
}

int	ft_print_str(va_list ap)
{
	char	*s;
	int		len;

	s = va_arg(ap, char *);
	if (s == NULL)
		s = "(null)";
	len = ft_putstr_fd(s, 1);
	return (len);
}

int	ft_print_pointer( va_list ap)
{
	unsigned long long		n;
	int						len;

	n = va_arg(ap, unsigned long long);
	ft_putstr_fd("0x", 1);
	len = ft_long_to_hex(n, HEXLOW);
	return (len + 2);
}

int	ft_print_i_d_u(va_list ap, char c)
{
	int		n;
	int		len;
	char	*s;

	n = va_arg(ap, int);
	if (c == 'u')
		s = ft_utoa(n);
	else
		s = ft_itoa(n);
	len = ft_putstr_fd(s, 1);
	free(s);
	return (len);
}

int	ft_print_percent(char c)
{
	if (c == '%')
		ft_putchar_fd('%', 1);
	return (1);
}
