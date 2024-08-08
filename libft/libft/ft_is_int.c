/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user4t <user4t@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:49:29 by carlosga          #+#    #+#             */
/*   Updated: 2024/08/08 19:00:34 by user4t           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	on_limit(char *str)
{
	int	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	len--;
	if (len > 11)
		return (-1);
	if (len == 11 && str[0] != '-')
		return (-1);
	else if (len < 10)
		return (0);
	else if (len == 10 && str[0] == '-')
		return (0);
	return (1);
}

int	ft_is_int(char *str)
{
	int		i;
	int		neg;
	char	*min;
	char	*max;

	min = "2147483648";
	max = "2147483647";
	i = 0;
	neg = 0;
	if (ft_isnumber(str) == 0 || on_limit(str) == -1)
		return (0);
	while (str[i] != '\0' && on_limit(str) == 1)
	{
		if (str[i] == '-')
		{
			neg = 1;
			str++;
		}
		if ((neg == 0 && str[i] > max[i]))
			return (0);
		else if ((neg == 1 && str[i] > min[i]))
			return (0);
		i++;
	}
	return (1);
}
