/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:00:16 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 18:05:48 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cd_error(char *directory)
{
	ft_putstr_fd(directory, 2);
	if (errno == ENOTDIR)
		ft_putstr_fd(": Not a directory\n", 2);
	else if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (errno == ELOOP)
		ft_putstr_fd(": Too many levels of symbolic links\n", 2);
	else if (errno == ENAMETOOLONG)
		ft_putstr_fd(": File name too long\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
}

bool	update_pwd(t_shell *shell, char *pwd, char **env)
{
	char	*env_str;

	env_str = ft_strjoin("OLDPWD=", pwd);
	if (!env_str)
		return (false);
	if (!add_env(&shell->env, env_str))
		return (free(env_str), false);
	free(env_str);
	pwd = get_pwd(shell, env);
	if (!pwd)
		return (false);
	env_str = ft_strjoin("PWD=", pwd);
	if (!env_str)
		return (false);
	if (!add_env(&shell->env, env_str))
		return (free(env_str), false);
	free(env_str);
	free(pwd);
	return (true);
}

int	ft_cd(t_shell *shell, char **env)
{
	char	*directory;
	char	*old_pwd;

	if (shell->context->args[1] == NULL)
	{
		directory = get_env_value("HOME", env, shell);
		if (!directory || directory[0] == '\0')
			return (free(directory), ft_putstr_fd("cd: HOME not set", 2), 1);
	}
	else
		directory = ft_strdup(shell->context->args[1]);
	old_pwd = get_pwd(shell, env);
	if (!old_pwd)
		return (ft_putstr_fd("An error has occurred\n", 2), 1);
	if (chdir(directory) != 0)
		return (print_cd_error(directory), free(old_pwd), free(directory), 1);
	free(directory);
	if (!update_pwd(shell, old_pwd, env))
		return (ft_putstr_fd("An error has occurred\n", 2), free(old_pwd), 1);
	free(old_pwd);
	return (0);
}
