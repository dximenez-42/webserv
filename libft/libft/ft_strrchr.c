/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:39:00 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:21:25 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t	i;

	i = ft_strlen(str);
	if ((unsigned char)c == '\0')
		return ((char *)str + i);
	while (i-- > 0)
	{
		if (*(str + i) == (unsigned char)c)
			return ((char *) str + i);
	}
	return ((char *) NULL);
}
