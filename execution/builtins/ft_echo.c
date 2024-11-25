/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:01:51 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 22:34:00 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_flag(char *str, char c)
{
	int	i;

	if (str[0] != '-')
		return (false);
	i = 1;
	while(str[i] == c)
		i++;
	return (str[i] == '\0');
}

int	ft_echo(t_context *context)
{
	char	**strs;
	int		fd;

	fd = context->output;
	if (fd == -1)
		fd = 1;
	strs = context->args + 1;
	if (context->args[1] && check_flag(context->args[1], 'n'))
		strs++;
	if (*strs)
		ft_putstr_fd(*strs++, fd);
	while (*strs)
	{
		ft_putchar_fd(' ', fd);
		ft_putstr_fd(*strs++, fd);
	}
	if (!context->args[1] || !check_flag(context->args[1], 'n'))
		ft_putchar_fd('\n', fd);
	return (0);
}

