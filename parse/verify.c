/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:02 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 16:35:00 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief a therapist
/// @param str someone who needs validation
/// @return you are valid
char	*validate_redirections(char *str)
{
	if (*str == *(str + 1))
		str++;
	str++;
	while (*str)
	{
		if (*str != ' ')
			break ;
		str++;
	}
	if (ft_strchr("<>|&", *str) != NULL)
		return (NULL);
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			str = ft_strchr(str + 1, *str);
		else if (ft_strchr(" <>|&", *str))
			break ;
		if (!str)
			return (NULL);
		str++;
	}
	return (str - 1);
}

/// @brief a therapist
/// @param str someone who needs validation
/// @return you are valid
char	*validate_pipe(char *str)
{
	str++;
	while (*str)
	{
		if (*str != ' ')
			break ;
		str++;
	}
	if (*str == '|' || *str == '\0')
		return (NULL);
	return (str - 1);
}

/// @brief a therapist
/// @param str someone who needs validation
/// @return you are valid
bool	validate_string(char *str)
{
	int	tokens;

	tokens = 0;
	while (*str)
	{
		if ((*str == '\'' || *str == '"') && ++tokens)
			str = ft_strchr(str + 1, *str);
		else if ((*str == '<' || *str == '>') && ++tokens)
			str = validate_redirections(str);
		else if (*str == '|')
		{
			str = validate_pipe(str);
			if (tokens == 0 || str == NULL)
				return (false);
			tokens = 0;
		}
		else if (*str != ' ' && *str != '\t')
			tokens++;
		if (str == NULL)
			return (false);
		str++;
	}
	return (true);
}
