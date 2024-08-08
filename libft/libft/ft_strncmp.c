/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 23:51:29 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:21:21 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	c;
	int		d;

	c = 0;
	d = 0;
	while ((c < n) && !d && (s1[c] != '\0') && (s2[c] != '\0'))
	{
		d = (unsigned char)s1[c] - (unsigned char)s2[c];
		c++;
	}
	if (c < n && !d && (s1[c] == '\0' || s2[c] == '\0'))
		d = (unsigned char)s1[c] - (unsigned char)s2[c];
	return (d);
}
