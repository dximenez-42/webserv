/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:10:43 by carlosga          #+#    #+#             */
/*   Updated: 2024/01/28 12:35:21 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strstr(char *str1, char *str2)
{
	int		i;
	int		j;
	size_t	len;

	i = 0;
	j = 0;
	len = ft_strlen(str1);
	while (i < (int)len && j < (int)ft_strlen(str2))
	{
		if (str1[i] == str2[j])
			j++;
		else
			j = 0;
		i++;
	}
	if (j == (int)ft_strlen(str2))
		return (i);
	else
		return (-1);
}
