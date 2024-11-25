/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rec_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:15 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 16:35:20 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	split_evenly(char *str, char *curr, t_tree **left, t_tree **right)
{
	char	*lstr;
	char	*rstr;

	lstr = ft_substr(str, 0, curr - str);
	rstr = ft_substr(str, curr - str + 1, -1);
	if (!lstr || !rstr)
		return (free(lstr), free(rstr), false);
	*left = create_node(lstr);
	*right = create_node(rstr);
	if (!*left || !*right)
		return (free(lstr), free(rstr),
			free_null((void **)left), free_null((void **)right), false);
	return (true);
}

bool	split_unevenly(char *str, char *curr, t_tree **left, t_tree **right)
{
	char	*lstr;
	char	*rstr;

	lstr = ft_strdup(str);
	if (!lstr)
		return (false);
	rstr = ft_substr(curr, 0, get_word(curr) - curr);
	if (!rstr)
		return (free(lstr), false);
	lstr = ft_strappend(lstr, curr + ft_strlen(rstr));
	if (!lstr)
		return (free(rstr), false);
	*left = create_node(lstr);
	*right = create_node(rstr);
	if (!*left || !*right)
		return (free(lstr), free(rstr),
			free_null((void **)left), free_null((void **)right), false);
	return (true);
}

bool	split_pipes(t_tree *head)
{
	char	*curr;

	if (head == NULL)
		return (1);
	if (head->cmd.type != WORD)
		return (split_pipes(head->left)
			&& split_pipes(head->right));
	curr = find_unescaped(head->cmd.str, "|");
	if (!curr)
		return (true);
	if (!split_evenly(head->cmd.str, curr, &head->left, &head->right))
		return (ft_putstr_fd("split pipe failed\n", 2), false);
	free_null((void **)&head->cmd.str);
	head->cmd.type = PIPE;
	return (split_pipes(head->left) && split_pipes(head->right));
}

bool	split_redirects(t_tree *head)
{
	char	*curr;

	if (head == NULL)
		return (1);
	if (head->cmd.type != WORD)
		return (split_redirects(head->left)
			&& split_redirects(head->right));
	curr = find_redirect(head->cmd.str, &head->cmd.type);
	if (!curr)
		return (true);
	*curr = '\0';
	curr++;
	if (head->cmd.type == HEREDOC || head->cmd.type == APPEND)
		curr++;
	if (!split_unevenly(head->cmd.str, curr, &head->left, &head->right))
		return (ft_putstr_fd("split redirects failed\n", 2), false);
	free_null((void **)&head->cmd.str);
	return (split_redirects(head->left));
}

t_tree	*construct_ast(char *str, char **env, t_shell *shell)
{
	t_tree	*head;

	head = create_node(str);
	if (!head)
		return (NULL);
	if (!split_pipes(head))
		return (ft_putstr_fd("split pipes failed\n", 2),
			free_tree(&head), NULL);
	if (!split_redirects(head))
		return (ft_putstr_fd("split redirects failed\n", 2),
			free_tree(&head), NULL);
	if (!expand_tree(head, env, shell))
		return (ft_putstr_fd("Cleanup failed\n", 2), free_tree(&head), NULL);
	if (!split_args(head))
		return (ft_putstr_fd("Cleanup failed\n", 2), free_tree(&head), NULL);
	if (!removing_quotes(head, env))
		return (ft_putstr_fd("Cleanup failed\n", 2), free_tree(&head), NULL);
	return (head);
}
