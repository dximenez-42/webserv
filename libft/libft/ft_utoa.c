/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:09:57 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:21:40 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_utoa(unsigned int n)
{
	long	nb;
	char	*s;
	size_t	i;

	nb = n;
	i = len(nb);
	s = (char *)malloc(sizeof(char) * (len(nb) + 1));
	if (!s)
		return (NULL);
	s[i--] = '\0';
	if (nb == 0)
		s[0] = '0';
	while (nb > 0)
	{
		s[i--] = 48 + (nb % 10);
		nb = nb / 10;
	}
	return (s);
}
