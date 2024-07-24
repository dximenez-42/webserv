/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr_rev.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 16:55:38 by carlosga          #+#    #+#             */
/*   Updated: 2023/10/09 17:27:30 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_substr_rev(char *str, char c)
{
	int		i;
	int		j;
	char	*substr;

	j = 0;
	i = ft_strlen(str);
	substr = ft_calloc(i + 1, sizeof(char));
	i--;
	while (str[i] != c && str[i] != '\0')
		i--;
	i++;
	while (str[i] != '\0')
	{
		substr[j] = str[i];
		i++;
		j++;
	}
	substr[j] = '\0';
	return (substr);
}
