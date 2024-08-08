/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 10:33:48 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:09:03 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	hex_len(unsigned long long n)
{
	int		i;

	i = 0;
	while (n != 0)
	{
		n = n / 16;
		i++;
	}
	return (i);
}

int	ft_long_to_hex(unsigned long long n, char	*arr)
{
	int			r;
	int			i;

	i = 0;
	r = n % 16;
	n = n / 16;
	if (n != 0)
		i = ft_long_to_hex(n, arr);
	ft_putchar_fd(arr[r], 1);
	i++;
	return (i);
}

int	ft_print_hex(va_list ap, char c)
{
	unsigned int	n;
	int				len;

	len = 0;
	n = va_arg(ap, int);
	if (c == 'X')
		len = ft_long_to_hex(n, HEXUP);
	else
		len = ft_long_to_hex(n, HEXLOW);
	return (len);
}
