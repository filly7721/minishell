/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:39 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 19:56:38 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	find_heredoc(t_tree *node)
{
	if (node->cmd.type == HEREDOC)
		return (true);
	if (node->cmd.type == WORD)
		return (false);
	return (find_heredoc(node->left));
}

bool	handle_input(t_tree *node, char **env, t_context *context)
{
	int	fd;

	if (node->right->cmd.strs[0] == NULL)
	{
		context->error = 1;
		return (ft_putstr_fd("ambiguous redirect\n", 2), false);
	}
	fd = open(node->right->cmd.strs[0], O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd(node->right->cmd.strs[0], 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
		context->error = 1;
		return (false);
	}
	if (find_heredoc(node->left))
		return (close(fd), traverse_tree(node->left, env, context));
	check_close(context->input);
	context->input = fd;
	return (traverse_tree(node->left, env, context));
}

bool	handle_output(t_tree *node, char **env, t_context *context, bool append)
{
	check_close(context->output);
	if (append)
		context->output = open(node->right->cmd.strs[0],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		context->output = open(node->right->cmd.strs[0],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (context->output == -1)
	{
		ft_putstr_fd(node->right->cmd.strs[0], 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
		context->error = 1;
		return (false);
	}
	return (traverse_tree(node->left, env, context));
}

bool	handle_word(t_tree *node, char **env, t_context *context)
{
	context->args = node->cmd.strs;
	node->cmd.strs = NULL;
	if (!context->args)
	{
		ft_putstr_fd("an error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	if (context->args[0])
	{
		if (is_builtin(context->args[0]))
			context->cmd = ft_strdup(context->args[0]);
		else
			context->cmd = get_path(context->args[0], env);
	}
	return (true);
}
