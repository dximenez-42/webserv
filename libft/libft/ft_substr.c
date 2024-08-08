/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 10:29:27 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:21:32 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			i;
	unsigned int	j;
	char			*str;

	if (len > ft_strlen(s))
		str = (char *)malloc(sizeof(*str) * (strlen(s) + 1));
	else
		str = (char *)malloc(sizeof(*str) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len && start < ft_strlen(s))
	{
		str[j] = s[start];
		i++;
		start++;
		j++;
	}
	str[j] = '\0';
	return (str);
}
