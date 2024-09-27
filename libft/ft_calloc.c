/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashalaab <ashalaab@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:43:37 by ashalaab          #+#    #+#             */
/*   Updated: 2024/06/14 12:43:37 by ashalaab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nelem, size_t elsize)
{
	size_t	size;
	void	*arr;

	size = nelem * elsize;
	if (elsize != 0 && size / elsize != nelem)
		return (NULL);
	arr = malloc(nelem * elsize);
	if (!arr)
		return (NULL);
	ft_memset(arr, 0, nelem * elsize);
	return (arr);
}
