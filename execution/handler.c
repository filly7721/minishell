#include "minishell.h"

bool	handle_input(t_tree *node, char **env, t_context *context)
{
	close(context->input);
	context->input = open(node->right->cmd.str, O_RDONLY);
	if (context->input == -1)
	{
		ft_putstr_fd("file error\n", 2);
		context->error = 1;
		return (false);
	}
	return (traverse_tree(node->left, env, context));
}

bool	handle_output(t_tree *node, char **env, t_context *context)
{
	close(context->output);
	context->output = open(node->right->cmd.str, O_WRONLY | O_CREAT, 0644);
	if (context->output == -1)
	{
		ft_putstr_fd("file error\n", 2);
		context->error = 1;
		return (false);
	}
	return (traverse_tree(node->left, env, context));
}

bool	handle_pipe(t_tree *node, char **env, t_context *context)
{
	pid_t	pid;
	int		fds[2];

	pipe(fds);
	context->output = fds[1];
	traverse_tree(node->left, env, context);
	pid = fork();
	if (pid == -1)
	{
		close(fds[0]); 
		close(fds[1]);
		return (false);
	}
	if (pid == 0)
	{
		close(fds[0]);
		execute_cmd(context, env);
	}
	close(fds[1]);
	reset_context(context);
	context->input = fds[0];
	return (traverse_tree(node->right, env, context));
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
	context->cmd = get_path(context->args[0], env);
	return (true);
}
