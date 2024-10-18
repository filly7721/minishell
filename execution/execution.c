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
	
	clear_context_list(context->next);
	if (context->error)
	{
		status = context->error;
		reset_context(context);
		free(context);
		exit(status);
	}
	if (context->input != -1)
		(dup2(context->input, 0), close(context->input));
	if (context->output != -1)
		(dup2(context->output, 1), close(context->output));
	execve(context->cmd, context->args, env);
	status = get_execution_error(context->args[0]);
	reset_context(context);
	free(context);
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

bool	execute_context(t_context *context, char **env, pid_t *pid)
{
	t_context	*next;
	
	while (context)
	{
		next = context->next;
		*pid = fork();
		if (*pid == -1)
			return (clear_context_list(context), ft_putstr_fd("an error has occurered\n", 2), false);
		if (*pid == 0)
			execute_cmd(context, env);
		reset_context(context);
		free(context);
		context = next;
	}
	return (true);
}

int	execute(t_tree *head, char **env)
{
	t_context	*context;
	pid_t		pid;
	int			status;

	context = malloc(sizeof(t_context));
	if (!context)
		return (ft_putstr_fd("An error has occurred: ", 2), 1);
	set_context(context);
	traverse_tree(head, env, context);
	free_tree(head);
	if (!execute_context(context, env, &pid))
		return (ft_putstr_fd("An error has occurred: ", 2), 1);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
