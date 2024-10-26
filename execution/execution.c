#include "minishell.h"

int	get_execution_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	if (errno == EACCES)
		return (ft_putstr_fd(": Permission denied\n", 2), 126);
	if (ft_strchr(cmd, '/') != NULL)
		ft_putstr_fd(": No such file or directory\n", 2);
	ft_putstr_fd(": Command not found\n", 2);
	return (127);
}

int	execute_cmd(t_context *context, char **env)
{
	int	status;

	clear_context_list(context->next);
	if (context->error)
	{
		status = context->error;
		free_context(context);
		return (status);
	}
	if (context->input != -1)
		(dup2(context->input, 0), close(context->input));
	if (context->output != -1)
		(dup2(context->output, 1), close(context->output));
	if (!context->args[0])
		(free_context(context), exit(0));
	if (context->cmd)
		execve(context->cmd, context->args, env);
	status = get_execution_error(context->args[0]);
	free_context(context);
	free_strs(env);
	return (status);
}

bool	traverse_tree(t_tree *node, char **env, t_context *context)
{
	if (node->cmd.type == PIPE)
		return (traverse_tree(node->left, env, context)
			&& traverse_tree(node->right, env, context->next));
	else if (node->cmd.type == WORD)
		return (handle_word(node, env, context));
	else if (node->cmd.type == HEREDOC)
		return (traverse_tree(node->left, env, context));
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
			return (clear_context_list(shell->context), free_strs(env),
				ft_putstr_fd("an error has occurered\n", 2), false);
		if (*pid == 0)
		{
			if (is_builtin(shell->context->cmd))
				status = execute_builtin(shell, env);
			else
			status = execute_cmd(shell->context, env);
			ft_lstclear(&shell->env, free);
			exit(status);
		}
		free_context(shell->context);
		shell->context = next;
	}
	return (true);
}

int	execute(t_shell *shell)
{
	pid_t		pid;
	int			status;
	char		**env;

	env = export_env(shell);
	shell->context = malloc(sizeof(t_context));
	if (!shell->context)
		return (free(env), ft_putstr_fd("An error has occurred: ", 2), 1);
	set_context(shell->context);
	premature_visitation(shell->tree, shell->context);
	traverse_tree(shell->tree, env, shell->context);
	free_tree(shell->tree);
	shell->tree = NULL;
	if (shell->context->next == NULL && is_builtin(shell->context->cmd))
		return (execute_builtin(shell, env));
	if (!execute_context(shell, env, &pid))
		return (ft_putstr_fd("An error has occurred: ", 2), 1);
	free_strs(env);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
