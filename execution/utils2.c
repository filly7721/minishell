/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:25:56 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 16:27:10 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_and_expand(char *str, char **env, t_shell *shell)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			str[i++] = '\0';
			str = expanded_str(str, str + i--, env, shell);
			if (!str)
				return (NULL);
			continue ;
		}
		i++;
	}
	return (str);
}
