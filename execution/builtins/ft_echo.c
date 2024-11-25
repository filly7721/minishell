/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:01:51 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 18:02:27 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_context *context)
{
	char	**strs;
	char	*flags;
	int		fd;
	int		i;

	fd = context->output;
	if (fd == -1)
		fd = 1;
	i = 1;
	flags = get_arg_flags(context->args, &i);
	if (!flags)
		return (ft_putstr_fd("An error has occurred\n", 2), 1);
	strs = context->args + i;
	if (*strs)
		ft_putstr_fd(*strs++, fd);
	while (*strs)
	{
		ft_putchar_fd(' ', fd);
		ft_putstr_fd(*strs++, fd);
	}
	if (ft_strchr(flags, 'n') == NULL)
		ft_putchar_fd('\n', fd);
	free(flags);
	return (0);
}
