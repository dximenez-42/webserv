/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:42:03 by carlosga          #+#    #+#             */
/*   Updated: 2023/10/18 14:41:44 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_count_words(char *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (count);
}

char	**ft_fill_array(char **aux, char *s, char c)
{
	size_t	i;
	size_t	j;
	int		k;
	size_t	s_len;

	i = 0;
	k = 0;
	s_len = ft_strlen(s);
	while (s[i])
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		j = i;
		while (s[i] != c && s[i] != '\0')
			i++;
		if (j >= s_len)
			aux[k++] = "\0";
		else
			aux[k++] = ft_substr(s, j, i - j);
	}
	return (aux);
}

char	**ft_split(char *s, char c)
{
	char	**aux;
	int		nwords;

	if (!s)
		return (NULL);
	nwords = ft_count_words(s, c);
	aux = malloc((nwords + 1) * sizeof(char *));
	if (aux == NULL)
		return (NULL);
	aux = ft_fill_array(aux, s, c);
	aux[nwords] = NULL;
	return (aux);
}
