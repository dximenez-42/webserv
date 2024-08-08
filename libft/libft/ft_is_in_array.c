/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_in_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 11:19:37 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:20:04 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	is_in_array(int n, int *arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (n == arr[i])
			return (0);
		i++;
	}
	return (1);
}
