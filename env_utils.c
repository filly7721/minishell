/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:23:47 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 15:26:23 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_shell(t_shell *shell)
{
	clear_context_list(&shell->context);
	ft_lstclear(&shell->env, free);
	free_tree(&shell->tree);
	free(shell);
}

int	shlvl_atoi(char *str)
{
	int	sign;
	int	res;

	while (*str == ' ')
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	res = 0;
	while (ft_isdigit(*str))
		res = res * 10 + *str++ - '0';
	if (*str != '\0')
		return (0);
	res++;
	if (sign == -1)
		return (-1);
	return (res);
}

void	remove_from_env(t_list **head, char *name)
{
	t_list	*tmp;

	if (*head == NULL)
		return ;
	if (ft_strncmp((*head)->content, name, ft_strlen(name)) == 0)
	{
		tmp = (*head)->next;
		ft_lstdelone(*head, free);
		*head = tmp;
		remove_from_env(head, name);
		return ;
	}
	remove_from_env(&((*head)->next), name);
}
