/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlosga <carlosga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 12:32:16 by carlosga          #+#    #+#             */
/*   Updated: 2024/01/28 12:34:32 by carlosga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "./libft.h"
# include <stdarg.h>

# define VARDEC "cspdiuxxX%%"
# define HEXLOW "0123456789abcdef"
# define HEXUP "0123456789ABCDEF"

int		ft_print_char(va_list ap);
int		ft_print_str(va_list ap);
int		ft_print_pointer(va_list ap);
int		ft_print_i_d_u(va_list ap, char c);
int		ft_print_hex(va_list ap, char c);
int		ft_print_percent(char c);
int		ft_long_to_hex(unsigned long long n, char *arr);
int		hex_len(unsigned long long int n);
int		ft_print_hex(va_list ap, char c);
int		ft_take_var(va_list ap, char c);
int		ft_printf(const char *s, ...);

#endif