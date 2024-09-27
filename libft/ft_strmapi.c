/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashalaab <ashalaab@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:43:40 by ashalaab          #+#    #+#             */
/*   Updated: 2024/06/14 12:43:40 by ashalaab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	size_t			size;
	char			*res;

	if (!s)
		return (NULL);
	size = ft_strlen(s);
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[size] = '\0';
	return (res);
}
