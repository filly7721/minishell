#include "minishell.h"

int	get_execution_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	if (errno == EACCES)
		return	(ft_putstr_fd(": Permission denied\n", 2), 126);
	if (ft_strchr(cmd, '/') != NULL)
		ft_putstr_fd(": No such file or directory\n", 2);
	ft_putstr_fd(": Command not found\n", 2);
	return (127);
}

void	execute_cmd(t_context *context, char **env)
{
	int	status;
	
	if (context->error)
	{
		status = context->error;
		reset_context(context);
		exit(status);
	}
	if (context->input != -1)
		(dup2(context->input, 0), close(context->input));
	if (context->output != -1)
		(dup2(context->output, 1), close(context->output));
	execve(context->cmd, context->args, env);
	status = get_execution_error(context->args[0]);
	reset_context(context);
	exit(status);
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
	if (pid == -1)
		return (reset_context(&context), ft_putstr_fd("an error has occurered\n", 2), 1);
	if (pid == 0)
		execute_cmd(&context, env);
	reset_context(&context);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
