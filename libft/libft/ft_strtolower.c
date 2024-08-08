/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtolower.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 12:06:47 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:21:30 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strtolower(char *s)
{
	int		aux;
	int		i;

	i = 0;
	while (s[i] != '\0')
	{
		aux = s[i];
		s[i] = ft_tolower(aux);
		i++;
	}
	return (s);
}
