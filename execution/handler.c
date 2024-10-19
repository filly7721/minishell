#include "minishell.h"

bool	handle_input(t_tree *node, char **env, t_context *context)
{
	close(context->input);
	context->input = open(node->right->cmd.str, O_RDONLY);
	if (context->input == -1)
	{
		ft_putstr_fd(node->right->cmd.str, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
		context->error = 1;
		return (false);
	}
	return (traverse_tree(node->left, env, context));
}

bool	handle_heredoc(t_tree *node, char **env, t_context *context)
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
	return (traverse_tree(node->left, env, context));
}

bool	handle_output(t_tree *node, char **env, t_context *context, bool append)
{
	close(context->output);
	if (append)
		context->output = open(node->right->cmd.str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		context->output = open(node->right->cmd.str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (context->output == -1)
	{
		ft_putstr_fd(node->right->cmd.str, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
		context->error = 1;
		return (false);
	}
	return (traverse_tree(node->left, env, context));
}

bool	handle_pipe(t_tree *node, char **env, t_context *context)
{
	int	fds[2];

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	close(context->output);
	context->output = fds[1];
	traverse_tree(node->left, env, context);
	context->next = malloc(sizeof(t_context));
	set_context(context->next);
	close(context->next->input);
	context->next->input = fds[0];
	return (traverse_tree(node->right, env, context->next));
}

bool	handle_word(t_tree *node, char **env, t_context *context)
{
	context->args = ft_split(node->cmd.str, ' ');
	if (!context->args)
	{
		ft_putstr_fd("an error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	if (context->args[0])
		context->cmd = get_path(context->args[0], env);
	return (true);
}
