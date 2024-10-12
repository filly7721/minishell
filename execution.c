#include "minishell.h"

int	execute_tree(t_tree *node, char **env);

void	handle_input(t_tree *node, char **env)
{
	int	file;

	file = open(node->right->cmd.str, O_RDONLY);
	if (file == -1)
	//TODO FILE ERROR HANDLING
		(ft_putstr_fd("file error\n", 2), exit(1));
	if (dup2(file, 0) == -1)
		(ft_putstr_fd("an error has occured\n", 2), exit(1));
	execute_tree(node->left, env);
}

void	handle_output(t_tree *node, char **env)
{
	int	file;

	file = open(node->right->cmd.str, O_WRONLY | O_CREAT, 0644);
	if (file == -1)
		(ft_putstr_fd("file error\n", 2), exit(1));
	if (dup2(file, 1) == -1)
		(ft_putstr_fd("an error has occured\n", 2), exit(1));
	execute_tree(node->left, env);
}

bool	connect_pipe(int fds[2], int fd)
{
	if (close(fds[1 - fd]) == -1)
		return (close(fds[fd]), false);
	if (dup2(fds[fd], fd) == -1)
		return (close(fds[fd]), false);
	if (close(fds[fd]) == -1)
		return (false);
	return (true);
}

void	handle_pipe(t_tree *node, char **env)
{
	int		fds[2];
	pid_t	pid;
	int		status;

	if (pipe(fds) == -1)
		(ft_putstr_fd("an error has occurred\n", 2), exit(1));
	pid = fork();
	if (pid == -1)
		(close(fds[0]), close(fds[1]), ft_putstr_fd("an error has occurred\n", 2), exit(1));
	if (pid == 0)
		if (!connect_pipe(fds, 1) || !execute_tree(node->left, env))
			(ft_putstr_fd("an error has occurred\n", 2), exit(1));
	pid = fork();
	if (pid == -1)
		(close(fds[0]), close(fds[1]), ft_putstr_fd("an error has occurred\n", 2), exit(1));
	if (pid == 0)
		if (!connect_pipe(fds, 0) || !execute_tree(node->right, env))
			(ft_putstr_fd("an error has occurred\n", 2), exit(1));
	(close(fds[1]), close(fds[0]));
	(waitpid(pid, &status, 0), wait(NULL));
	exit(WEXITSTATUS(status));
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

void	handle_word(t_tree *node, char **env)
{
	char	*cmd;
	char	**strs;

	strs = ft_split(node->cmd.str, ' ');
	if (!strs)
		(ft_putstr_fd("an error has occurred\n", 2), exit(1));
	cmd = get_path(strs[0], env);
	if (!cmd)
		(ft_putstr_fd("an error has occurred\n", 2), exit(1));
	execve(cmd, strs, env);
	ft_putstr_fd("an error has occured executing ", 2);
	// TODO handle error codes
	ft_putendl_fd(strs[0], 2);
	free_strs(strs);
	free(cmd);
	exit(127);
}

int	execute_tree(t_tree *node, char **env)
{
	if (node->cmd.type == PIPE)
		handle_pipe(node, env);
	else if (node->cmd.type == WORD)
		handle_word(node, env);
	else if (node->cmd.type == INPUT)
		handle_input(node, env);
	else if (node->cmd.type == OUTPUT)
		handle_output(node, env);
	else
		ft_putstr_fd("unknown type in tree\n", 2);
	return (1);
}

int		execute(t_tree *node, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		execute_tree(node, env);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}