/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 23:54:34 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:08:35 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

double	ft_atof(const char *str)
{
	double		n;
	int			nint;
	int			i;
	double		div;

	nint = ft_atoi(str);
	i = 0;
	div = 1;
	while (str[i] != ',' && str[i] != '.' && str[i] != '\0')
		i++;
	i++;
	while (str[i] != '\0')
	{
		i++;
		div /= 10;
	}
	n = nint * div;
	return (n);
}
