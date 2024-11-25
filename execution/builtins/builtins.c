/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:28 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 20:29:19 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_arg_flags(char **args, int *i)
{
	char	*flags;
	int		j;

	flags = ft_strdup("");
	while (args[*i] != NULL && *(args[*i]) == '-')
	{
		j = 1;
		while (args[*i][j])
		{
			if (ft_strchr(flags, args[*i][j]) == NULL)
			{
				flags = ft_strappend(flags, " ");
				if (!flags)
					return (NULL);
				flags[ft_strlen(flags) - 1] = args[*i][j];
			}
			j++;
		}
		(*i)++;
	}
	return (flags);
}

int	execute_builtin(t_shell *shell, char **env)
{
	int	status;

	clear_context_list(&shell->context->next);
	status = 1;
	signal(SIGPIPE, SIG_IGN);
	if (ft_strncmp(shell->context->cmd, "echo", -1) == 0)
		status = ft_echo(shell->context);
	else if (ft_strncmp(shell->context->cmd, "pwd", -1) == 0)
		status = ft_pwd(shell, env, shell->context);
	else if (ft_strncmp(shell->context->cmd, "unset", -1) == 0)
		status = ft_unset(shell);
	else if (ft_strncmp(shell->context->cmd, "export", -1) == 0)
		status = ft_export(shell);
	else if (ft_strncmp(shell->context->cmd, "cd", -1) == 0)
		status = ft_cd(shell, env);
	else if (ft_strncmp(shell->context->cmd, "env", -1) == 0)
		status = ft_env(shell, env);
	else if (ft_strncmp(shell->context->cmd, "exit", -1) == 0)
		status = ft_exit(shell, env);
	free_context(shell->context);
	signal(SIGPIPE, SIG_DFL);
	shell->context = NULL;
	free_strs(env);
	return (status);
}

bool	is_builtin(char *str)
{
	if (!str)
		return (false);
	if (ft_strncmp(str, "echo", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "pwd", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "unset", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "export", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "cd", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "env", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "exit", -1) == 0)
		return (true);
	return (false);
}
