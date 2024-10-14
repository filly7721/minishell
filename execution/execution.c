#include "minishell.h"

void	execute_cmd(t_context *context, char **env)
{
	if (context->is_error)
	{
		reset_context(context);
		exit(1);
	}
	if (context->input != -1)
		(dup2(context->input, 0), close(context->input));
	if (context->output != -1)
		(dup2(context->output, 1), close(context->output));
	execve(context->cmd, context->args, env);
	ft_putstr_fd("an error has occurered executing: ", 2);
	ft_putendl_fd(context->cmd, 2);
	free_strs(context->args);
	free(context->cmd);
	exit(127);
}

bool	traverse_tree(t_tree *node, char **env, t_context *context)
{
	if (node->cmd.type == PIPE)
		return (handle_pipe(node, env, context));
	else if (node->cmd.type == WORD)
		return (handle_word(node, env, context));
	else if (node->cmd.type == INPUT)
		return (handle_input(node, env, context));
	else if (node->cmd.type == OUTPUT)
		return (handle_output(node, env, context));
	else
		ft_putstr_fd("unknown type in tree\n", 2);
	return (false);
}

int	execute(t_tree *node, char **env)
{
	t_context	context;
	pid_t		pid;
	int			status;

	set_context(&context);
	traverse_tree(node, env, &context);
	pid = fork();
	if (pid == 0)
		execute_cmd(&context, env);
	reset_context(&context);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	return (WEXITSTATUS(status));
}
