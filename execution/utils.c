#include "minishell.h"

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