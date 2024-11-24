#include "minishell.h"

bool	is_whitespace(char *str)
{
	while (*str == ' ' || * str == '\t')
		str++;
	return (*str == '\0');
}

int	get_execution_error(char *cmd)
{
	int	errnum;
	DIR	*dir;

	errnum = errno;
	ft_putstr_fd("megashell: ", 2);
	ft_putstr_fd(cmd, 2);
	dir = opendir(cmd);
	if (dir)
		return (closedir(dir), ft_putstr_fd(": is a directory\n", 2), 126);
	if (is_whitespace(cmd))
		return (ft_putstr_fd(": Command not found\n", 2), 127);
	if (errnum == EACCES)
		return (ft_putstr_fd(": Permission denied\n", 2), 126);
	if (ft_strchr(cmd, '/') != NULL)
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": Command not found\n", 2);
	return (127);
}

int	execute_cmd(t_context *context, char **env)
{
	int	status;

	clear_context_list(&context->next);
	if (context->error)
	{
		status = context->error;
		free_strs(env);
		return (status);
	}
	if (context->input != -1)
		(dup2(context->input, 0), close(context->input));
	if (context->output != -1)
		(dup2(context->output, 1), close(context->output));
	if (!context->args[0])
		return (free_strs(env), 0);
	if (context->cmd)
		execve(context->cmd, context->args, env);
	status = get_execution_error(context->args[0]);
	free_strs(env);
	return (status);
}

bool	traverse_tree(t_tree *node, char **env, t_context *context)
{
	if (node->cmd.type == PIPE)
		return (traverse_tree(node->left, env, context)
			&& traverse_tree(node->right, env, context->next));
	else if (node->cmd.type == HEREDOC)
		return (traverse_tree(node->left, env, context));
	else if (node->cmd.type == WORD)
		return (handle_word(node, env, context));
	else if (node->cmd.type == INPUT)
		return (handle_input(node, env, context));
	else if (node->cmd.type == APPEND)
		return (handle_output(node, env, context, true));
	else if (node->cmd.type == OUTPUT)
		return (handle_output(node, env, context, false));
	else
		ft_putstr_fd("unknown type in tree\n", 2);
	return (false);
}

bool	execute_context(t_shell *shell, char **env, pid_t *pid)
{
	t_context	*next;
	int			status;

	while (shell->context)
	{
		next = shell->context->next;
		*pid = fork();
		if (*pid == -1)
			return (clear_context_list(&shell->context), free_strs(env),
				ft_putstr_fd("an error has occurered\n", 2), false);
		if (*pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (is_builtin(shell->context->cmd))
				status = execute_builtin(shell, env);
			else
				status = execute_cmd(shell->context, env);
			clear_shell(shell);
			exit(status);
		}
		free_context(shell->context);
		shell->context = next;
	}
	return (true);
}

void	print_signal_errors(int status)
{
	if (status == SIGINT)
		ft_putstr_fd("\n", 2);
	else if (status == SIGQUIT)
	{
		ft_putstr_fd("Quit: ", 2);
		ft_putnbr_fd(status, 2);
		ft_putstr_fd(": womp womp\n", 2);
	}
	else if (status == SIGSEGV)
	{
		ft_putstr_fd("Segmentation fault: ", 2);
		ft_putnbr_fd(status, 2);
		ft_putstr_fd(": is bro dereferencing null 💀\n", 2);
	}
}

int	execute(t_shell *shell, char **env)
{
	pid_t		pid;
	int			status;

	shell->context = create_context();
	if (!shell->context)
		return (ft_putstr_fd("An error has occurred: ", 2), 1);
	signal(SIGINT, SIG_IGN);
	if (premature_visitation(shell->tree, shell->context, env, shell) == false)
		return (clear_context_list(&shell->context), free_tree(&shell->tree), free_strs(env), 0);
	traverse_tree(shell->tree, env, shell->context);
	free_tree(&shell->tree);
	if (shell->context->next == NULL && is_builtin(shell->context->cmd))
		return (execute_builtin(shell, env));
	if (!execute_context(shell, env, &pid))
		return (ft_putstr_fd("An error has occurred: ", 2), free_strs(env), 1);
	free_strs(env);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFSIGNALED(status))
		return (print_signal_errors(WTERMSIG(status)), 128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
