/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:40:35 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:21:23 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strnstr(const char	*big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (ft_strlen(little) == 0)
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		if (big[i] == little[0])
		{
			while (little[j] && i + j < len)
			{
				if (little[j] != big[i + j])
					break ;
				j++;
			}
			if (!little[j])
				return ((char *)big + i);
		}
		i++;
	}
	if (little[0] == 0)
		return ((char *)big);
	return (NULL);
}
