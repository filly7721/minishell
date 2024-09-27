/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashalaab <ashalaab@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:43:37 by ashalaab          #+#    #+#             */
/*   Updated: 2024/06/14 12:43:37 by ashalaab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	res;

	res = 0;
	sign = 1;
	i = 0;
	while (is_whitespace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
		res = res * 10 + nptr[i++] - '0';
	return (sign * res);
}
