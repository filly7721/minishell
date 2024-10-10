#include "minishell.h"
#include <errno.h>

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	if (strs == NULL)
		return ;
	while (strs[i] != NULL)
		free(strs[i++]);
	free(strs);
}

static char	*ft_strappend(char *str1, char *str2)
{
	char	*res;

	if (str1 == NULL || str2 == NULL)
		return (NULL);
	res = ft_strjoin(str1, str2);
	free(str1);
	return (res);
}

bool	handle_input(t_tree *node, char **env)
{
	int	file;

	file = open(node->right->cmd.str, O_RDONLY);
	if (file == -1)
		(ft_putstr_fd("file error", 2), exit(1));
	if (dup2(file, 0) == -1)
		(ft_putendl_fd("an error has occured", 2), exit(1));
	return (execute(node->left, env));
}

bool	handle_output(t_tree *node, char **env)
{
	int	file;

	file = open(node->right->cmd.str, O_WRONLY | O_CREAT, 0644);
	if (file == -1)
		(ft_putstr_fd("file error", 2), exit(1));
	if (dup2(file, 1) == -1)
		(ft_putendl_fd("an error has occured", 2), exit(1));
	return (execute(node->left, env));
}

bool	handle_pipe(t_tree *node, char **env)
{
	int		fds[2];
	pid_t	pid;

	if (pipe(fds) == -1)
		return (false);
	pid = fork();
	if (pid == -1)
		return (close(fds[0]), close(fds[1]), false);
	if (pid == 0)
	{
		printf("output from %s\n", node->left->cmd.str);
		close(fds[0]);
		if (dup2(fds[1], 1) == -1)
			(printf("an error has occurerd"), exit(1));
		close(fds[1]);
		return (execute(node->left, env));
	}
	printf("input to %s\n", node->right->cmd.str);
	close(fds[1]);
	if (dup2(fds[0], 0) == -1)
		(printf("an error has occurerd"), exit(1));
	close(fds[0]);
	return (execute(node->right, env));
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
		return NULL;
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
	return (NULL);
}

bool	handle_cmd(t_tree *node, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(node->cmd.str, ' ');
	if (!cmd)
		return (false);
	path = get_path(cmd[0], env);
	if (!path)
		return (false);
	execve(path, cmd, env);
	ft_putstr_fd("an error has occured executing ", 2);
	ft_putendl_fd(cmd[0], 2);
	free_strs(cmd);
	free(path);
	exit(127);
}

bool	execute(t_tree *node, char **env)
{
	if (!node)
		return (1);
	if (node->cmd.type == PIPE)
		return (handle_pipe(node, env));
	if (node->cmd.type == INPUT)
		return (handle_input(node, env));
	if (node->cmd.type == OUTPUT)
		return (handle_output(node, env));
	return (handle_cmd(node, env));
}




// if (fcntl(0, F_GETFD) != -1) {
// return 1; // Valid file descriptor
// } else {
//     if (errno == EBADF) {
//         return 0; // Invalid file descriptor
//     } else {
//         perror("fcntl failed");
//         return -1; // Some other error occurred
//     }