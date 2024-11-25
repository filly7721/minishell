/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:01:38 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 18:02:50 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_shell *shell)
{
	int		i;
	int		j;
	char	*name;

	i = 1;
	while (shell->context->args[i])
	{
		j = 0;
		if (!ft_isalpha(shell->context->args[i][j]) &&
			shell->context->args[i][j] != '_')
			return (ft_putstr_fd("Invalid variable name\n", 2), 1);
		while (ft_isalnum(shell->context->args[i][j]) ||
			shell->context->args[i][j] == '_')
			j++;
		if (shell->context->args[i][j] != '\0')
			return (ft_putstr_fd("Invalid variable name\n", 2), 1);
		name = ft_strjoin(shell->context->args[i], "=");
		if (!name)
			return (ft_putstr_fd("An error has occurred\n", 2), 1);
		remove_from_env(&shell->env, name);
		free(name);
		i++;
	}
	return (0);
}
