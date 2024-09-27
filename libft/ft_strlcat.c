/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashalaab <ashalaab@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:43:40 by ashalaab          #+#    #+#             */
/*   Updated: 2024/06/14 12:43:40 by ashalaab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	if (size == 0)
		return (ft_strlen(src));
	j = ft_strlen(dst);
	if (size <= j)
		return (ft_strlen(src) + size);
	i = 0;
	while (src[i] != '\0' && i < size - j - 1)
	{
		dst[j + i] = src[i];
		i++;
	}
	dst[j + i] = '\0';
	return (ft_strlen(src) + j);
}
