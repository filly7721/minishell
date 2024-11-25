/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:17:54 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 15:17:55 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(char *str, char c)
{
	char	*curr;

	curr = str;
	while (*curr != '\0' && *curr != c)
	{
		if (*curr == '"' || *curr == '\'')
			curr = ft_strchr(curr + 1, *curr);
		curr++;
	}
	return (ft_substr(str, 0, curr - str));
}

int	count_words(char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			count++;
		while (*str != '\0' && *str != c)
		{
			if (*str == '"' || *str == '\'')
				str = ft_strchr(str + 1, *str);
			str++;
		}
	}
	return (count);
}

char	**quote_split(char *str, char c)
{
	char	**res;
	int		i;

	res = ft_calloc(count_words(str, c) + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			res[i++] = extract_word(str, c);
		if (*str && res[i - 1] == NULL)
			return (free_strs(res), NULL);
		while (*str != '\0' && *str != c)
		{
			if (*str == '"' || *str == '\'')
				str = ft_strchr(str + 1, *str);
			str++;
		}
	}
	return (res);
}

void	check_close(int fd)
{
	if (fd < 0)
		return ;
	close(fd);
}
