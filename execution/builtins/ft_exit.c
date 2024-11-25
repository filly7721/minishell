/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:40:27 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 20:02:26 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	exit_atoi(char *str, int *num)
{
	int	sign;
	int	i;

	i = 0;
	sign = 1;
	*num = 0;
	while (str[i] == ' ')
		i++;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (ft_isdigit(str[i]))
		*num = ((*num) * 10 + str[i++] - '0') % 256;
	while (str[i] == ' ')
		i++;
	if (str[i])
		return (false);
	if (sign == -1)
		*num = 256 - *num;
	return (true);
}

int	ft_exit(t_shell *shell, char **env)
{
	int	status;

	if (shell->context->args[1] == NULL)
	{
		status = shell->status;
		(free_strs(env), clear_shell(shell));
		(ft_putendl_fd("exit", 2), exit(status));
	}
	if (exit_atoi(shell->context->args[1], &status) == false)
	{
		ft_putstr_fd("megashell: ", 2);
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(shell->context->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		(free_strs(env), clear_shell(shell));
		exit(255);
	}
	if (shell->context->args[2])
		return (ft_putstr_fd("megashell: exit: too many arguments\n", 2), 1);
	free_strs(env);
	clear_shell(shell);
	exit(status % 256);
}
