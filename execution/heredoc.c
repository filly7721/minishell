#include "minishell.h"

bool	handle_heredoc(t_tree *node, t_context *context)
{
	int		fds[2];
	char	*str;

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	str = readline("heredoc> ");
	while (str && ft_strncmp(str, node->right->cmd.str, -1) != 0)
	{
		ft_putendl_fd(str, fds[1]);
		free(str);
		str = readline("heredoc> ");
	}
	close(context->input);
	context->input = fds[0];
	close(fds[1]);
	return (true);
}

bool	handle_pipe(t_tree *node, t_context *context)
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
	return (premature_visitation(node->right, context->next));
}

bool	premature_visitation(t_tree *node, t_context *context)
{
	if (node->cmd.type == WORD)
		return (true);
	if (node->cmd.type == HEREDOC)
		if (!handle_heredoc(node, context))
			return (false);
	if (!premature_visitation(node->left, context))
		return (false);
	if (node->cmd.type == PIPE)
		if (!handle_pipe(node, context))
			return (false);
	return (true);
}
