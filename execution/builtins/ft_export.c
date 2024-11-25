/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:41:38 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 17:44:06 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_export(char *str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	while (*str != '\0' && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}

void	print_export(t_list *list, int fd)
{
	char	*end;
	t_list	*curr;

	curr = list;
	while (curr)
	{
		end = ft_strchr(curr->content, '=') + 1;
		ft_putstr_fd("declare -x ", fd);
		write(fd, curr->content, end - (char *)curr->content);
		ft_putchar_fd('"', fd);
		ft_putstr_fd(end, fd);
		ft_putstr_fd("\"\n", fd);
		curr = curr->next;
	}
}

int	sort_print(t_list *list, int fd)
{
	int		count;
	char	*temp;
	t_list	*curr;

	if (!list)
		return (0);
	count = ft_lstsize(list);
	while (count)
	{
		curr = list;
		while (curr->next)
		{
			if (ft_strncmp(curr->content, curr->next->content, -1) > 0)
			{
				temp = curr->content;
				curr->content = curr->next->content;
				curr->next->content = temp;
			}
			curr = curr->next;
		}
		count--;
	}
	print_export(list, fd);
	return (ft_lstclear(&list, free), 0);
}

int	ft_export(t_shell *shell)
{
	int		i;
	int		fd;

	fd = shell->context->output;
	if (fd == -1)
		fd = 1;
	i = 1;
	while (shell->context->args[i])
	{
		if (!validate_export(shell->context->args[i]))
			return (ft_putstr_fd("Invalid variable name\n", 2), 1);
		if (!add_env(&shell->env, shell->context->args[i]))
			return (ft_putstr_fd("An error has occurred\n", 2), 1);
		i++;
	}
	if (shell->context->args[1] == NULL)
		return (sort_print(ft_lstmap(shell->env,
					(void *(*)(void *))ft_strdup, free), fd));
	return (0);
}
