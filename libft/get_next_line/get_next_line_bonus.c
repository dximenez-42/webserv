/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:40:00 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/26 13:08:58 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line_bonus.h"

char	*ft_readfd(int fd, char *stash)
{
	char	*buffer;
	int		read_bytes;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	read_bytes = 1;
	while (gnl_strchr(stash, '\n') == -1 && read_bytes != 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(buffer);
			free(stash);
			return (NULL);
		}
		buffer[read_bytes] = '\0';
		stash = gnl_strjoin(stash, buffer);
	}
	free(buffer);
	return (stash);
}

char	*ft_set_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_set_stash(char *stash)
{
	int		i;
	int		j;
	char	*s;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	s = (char *)malloc(sizeof(char) * (ft_strlen(stash) - i + 1));
	if (!s)
		return (NULL);
	i++;
	j = 0;
	while (stash[i])
		s[j++] = stash[i++];
	s[j] = '\0';
	free(stash);
	return (s);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash[MAX_FD];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	stash[fd] = ft_readfd(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = ft_set_line(stash[fd]);
	stash[fd] = ft_set_stash(stash[fd]);
	return (line);
}
