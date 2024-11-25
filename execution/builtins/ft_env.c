/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:03:20 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 18:06:55 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell, char **env)
{
	int		fd;

	fd = shell->context->output;
	if (fd == -1)
		fd = 1;
	while (*env)
		ft_putendl_fd(*env++, fd);
	return (0);
}
