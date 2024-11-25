/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:36 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 16:26:35 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int signum)
{
	g_sig = signum;
	write(1, "\n", 1);
	close(0);
}

void	heredoc_child(int fds[2], char *delimiter, t_shell *shell, char **env)
{
	char	*str;

	close(fds[0]);
	signal(SIGINT, heredoc_sigint);
	str = readline("heredoc> ");
	while (str && ft_strncmp(str, delimiter, -1) != 0)
	{
		str = find_and_expand(str, env, shell);
		ft_putendl_fd(str, fds[1]);
		free(str);
		str = readline("heredoc> ");
	}
	close(fds[1]);
	free_strs(env);
	clear_shell(shell);
	exit(g_sig == SIGINT);
}

bool	handle_heredoc(t_tree *node, t_context *context,
		char **env, t_shell *shell)
{
	int		fds[2];
	int		status;
	int		pid;

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	pid = fork();
	if (pid == 0)
		heredoc_child(fds, node->right->cmd.strs[0], shell, env);
	waitpid(pid, &status, 0);
	close(context->input);
	context->input = fds[0];
	close(fds[1]);
	return (WEXITSTATUS(status) == 0);
}

bool	handle_pipe(t_tree *node, t_context *context,
	char **env, t_shell *shell)
{
	int	fds[2];

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	context->next = create_context();
	if (!context->next)
	{
		close(fds[0]);
		close(fds[1]);
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	context->output = fds[1];
	context->next->input = fds[0];
	return (premature_visitation(node->right, context->next, env, shell));
}

bool	premature_visitation(t_tree *node, t_context *context,
	char **env, t_shell *shell)
{
	if (node->cmd.type == WORD)
		return (true);
	if (node->cmd.type == HEREDOC)
		if (!handle_heredoc(node, context, env, shell))
			return (false);
	if (!premature_visitation(node->left, context, env, shell))
		return (false);
	if (node->cmd.type == PIPE)
		if (!handle_pipe(node, context, env, shell))
			return (false);
	return (true);
}
