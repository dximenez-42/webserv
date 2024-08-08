/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 17:19:57 by carlosga          #+#    #+#             */
/*   Updated: 2023/09/27 16:53:25 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef MAX_FD
#  define MAX_FD 256
# endif

char		*ft_readfd(int fd, char *stash);
char		*ft_split_stash(char **stash);
char		*get_next_line(int fd);
char		*shrink_bufffer(char *buf, char *line);
char		*expand_buffer(char buf, int fd);
char		*ft_fill_stash(char *s1, char *s2);
char		*ft_strdup(const char *s);
int			gnl_strchr(char *str, int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*gnl_strjoin(char *s1, char *s2);
#endif