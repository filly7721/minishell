/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:43:31 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 18:05:35 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_shell *shell, char **env, t_context *context)
{
	int		fd;
	char	*str;

	fd = context->output;
	if (fd == -1)
		fd = 1;
	str = get_pwd(shell, env);
	if (!str)
		return (ft_putstr_fd("An error has occurred\n", 2), 1);
	ft_putendl_fd(str, 2);
	free(str);
	return (0);
}

char	*get_pwd(t_shell *shell, char **env)
{
	char	*buff;
	size_t	size;

	size = 32;
	while (1)
	{
		buff = malloc(size);
		if (!buff)
			return (NULL);
		if (getcwd(buff, size) != NULL)
			break ;
		free(buff);
		if (errno == ENOENT)
			return (find_and_expand(ft_strdup("$PWD"), env, shell));
		size *= 2;
	}
	return (ft_strappend(buff, ""));
}
