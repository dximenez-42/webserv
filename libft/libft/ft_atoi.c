/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 23:54:34 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:19:58 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_check_max_min(long long int n)
{
	if (n > 2147483647)
		return (1);
	else if (n < -2147483648)
		return (0);
	else
		return ((int)n);
}

int	ft_atoi(const char *str)
{
	long long int	n;
	int				neg;

	n = 0;
	neg = 1;
	while (*str == '\t' || *str == '\n' || *str == '\r'
		|| *str == '\v' || *str == '\f' || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			neg *= -1;
		str++;
	}
	while ((*str >= '0' && *str <= '9') || *str == '.')
	{
		if ((*(str + 1) < '0' || *(str + 1) > '9') && *(str + 1) != '.')
			n = (n + (*str - 48));
		else if (*str != '.')
			n = (n + (*str - 48)) * 10;
		str++;
	}
	return ((ft_check_max_min(n)) * neg);
}
