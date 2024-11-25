/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:17:39 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 15:25:45 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_lst(t_list *list, char *str, int len)
{
	while (list != NULL)
	{
		if (ft_strncmp(list->content, str, len) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

bool	add_env(t_list **list, char *str)
{
	char	*copy;
	t_list	*temp;

	if (ft_strchr(str, '=') != NULL)
		copy = ft_strdup(str);
	else
		copy = ft_strjoin(str, "=");
	if (!copy)
		return (false);
	temp = find_lst(*list, copy, ft_strchr(copy, '=') - copy);
	if (temp)
	{
		free(temp->content);
		temp->content = copy;
		return (true);
	}
	temp = ft_lstnew(copy);
	if (!temp)
		return (free(copy), false);
	ft_lstadd_front(list, temp);
	return (true);
}

char	**export_env(t_shell *shell)
{
	t_list	*curr;
	char	**env;
	int		i;

	curr = shell->env;
	env = ft_calloc(sizeof(char *), ft_lstsize(curr) + 1);
	if (!env)
		return (NULL);
	i = 0;
	while (curr)
	{
		env[i] = ft_strdup((char *)(curr->content));
		if (!env[i])
			return (free_strs(env), NULL);
		curr = curr->next;
		if (*(ft_strchr(env[i], '=') + 1) == '\0')
			free_null((void **)&env[i]);
		else
			i++;
	}
	return (env);
}

bool	env_init(t_shell *shell)
{
	int		shlvl;
	t_list	*node;
	char	*str;
	char	*num;

	node = find_lst(shell->env, "SHLVL=", 6);
	shlvl = 1;
	if (node)
		shlvl = shlvl_atoi((char *)node->content + 6);
	if (shlvl < 0)
		shlvl = 0;
	num = ft_itoa(shlvl);
	if (!num)
		return (false);
	str = ft_strjoin("SHLVL=", num);
	free(num);
	if (!str || !add_env(&shell->env, str))
		return (free(str), false);
	free(str);
	node = find_lst(shell->env, "OLDPWD=", 7);
	if (!node)
		return (add_env(&shell->env, "OLDPWD"));
	return (true);
}

t_shell	*create_shell(char **env)
{
	t_shell	*shell;
	t_list	*node;
	char	*str;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (NULL);
	while (*env)
	{
		str = ft_strdup(*env);
		if (!str)
			return (ft_lstclear(&shell->env, free), free(shell), NULL);
		node = ft_lstnew(str);
		if (!node)
			return (free(str), ft_lstclear(&shell->env, free),
				free(shell), NULL);
		ft_lstadd_back(&shell->env, node);
		env++;
	}
	if (env_init(shell) == false)
		return (clear_shell(shell), NULL);
	return (shell);
}
