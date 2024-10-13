#include "minishell.h"

bool	traverse_tree(t_tree *node, char **env, t_context *context);

void	set_context(t_context *context)
{
	context->args = NULL;
	context->cmd = NULL;
	context->input = -1;
	context->output = -1;
	context->is_error = false;
}

void	reset_context(t_context *context)
{
	free_strs(context->args);
	context->args = NULL;
	free(context->cmd);
	context->cmd = NULL;
	close(context->input);
	context->input = -1;
	close(context->output);
	context->output = -1;
	context->is_error = false;
}

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

bool	handle_input(t_tree *node, char **env, t_context *context)
{
	close(context->input);
	context->input = open(node->right->cmd.str, O_RDONLY);
	if (context->input == -1)
	{
		ft_putstr_fd("file error\n", 2);
		context->is_error = true;
		return (false);
	}
	return (traverse_tree(node->left, env, context));
}

bool	handle_output(t_tree *node, char **env, t_context *context)
{
	close(context->output);
	context->output = open(node->right->cmd.str, O_WRONLY | O_CREAT);
	if (context->input == -1)
	{
		ft_putstr_fd("file error\n", 2);
		context->is_error = true;
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

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (*env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (!env)
		return (NULL);
	paths = ft_split(*env + 5, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		path = ft_strappend(ft_strjoin(paths[i], "/"), cmd);
		if (!path)
			return (free_strs(paths), NULL);
		if (access(path, X_OK) == 0)
			return (free_strs(paths), path);
		free(path);
	}
	return (free_strs(paths), NULL);
}

bool	handle_word(t_tree *node, char **env, t_context *context)
{
	context->args = ft_split(node->cmd.str, ' ');
	if (!context->args)
	{
		ft_putstr_fd("an error has occurred\n", 2);
		context->is_error = true;
		return (false);
	}
	context->cmd = get_path(context->args[0], env);
	if (!context->cmd)
	{
		ft_putstr_fd("an error has occurred\n", 2);
		context->is_error = true;
		return (false);
	}
	return (true);
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
