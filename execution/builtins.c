#include "minishell.h"

int	ft_echo(t_context *context)
{
	char	**strs;
	int		fd;

	fd = context->output;
	if (fd == -1)
		fd = 1;
	strs = context->args + 1;
	if (context->args[1] && ft_strncmp(context->args[1], "-n", -1) == 0)
		strs++;
	if (*strs)
		ft_putstr_fd(*strs++, fd);
	while (*strs)
	{
		ft_putchar_fd(' ', fd);
		ft_putstr_fd(*strs++, fd);
	}
	if (!context->args[1] || ft_strncmp(context->args[1], "-n", -1) != 0)
		ft_putchar_fd('\n', fd);
	return (0);
}

int	ft_pwd(t_context *context)
{
	int		fd;
	char	*buff;
	size_t	size;

	fd = context->output;
	if (fd == -1)
		fd = 1;
	size = 32;
	while (1)
	{
		buff = malloc(size);
		if (!buff)
			return (ft_putstr_fd("An error has occured\n", 2), 1);
		if (getcwd(buff, size) != NULL)
			break ;
		free(buff);
		size *= 2;
	}
	ft_putendl_fd(buff, 2);
	free(buff);
	return (0);
}

int	ft_unset(t_shell *shell)
{
	int		i;
	char	*name;

	i = 1;
	while (shell->context->args[i])
	{
		name = ft_strjoin(shell->context->args[i], "=");
		if (!name)
			return (ft_putstr_fd("An error has occured\n", 2), 1);
		remove_from_env(&shell->env, name);
		free(name);
		i++;
	}
	return (0);
}

int	execute_builtin(t_shell *shell, char **env)
{
	int	status;

	clear_context_list(shell->context->next);
	if (ft_strncmp(shell->context->cmd, "echo", -1) == 0)
		status = ft_echo(shell->context);
	else if (ft_strncmp(shell->context->cmd, "pwd", -1) == 0)
		status = ft_pwd(shell->context);
	else if (ft_strncmp(shell->context->cmd, "unset", -1) == 0)
		status = ft_unset(shell);
	else
	{
		ft_putstr_fd("unhandled builtin", 2);
		status = 1;
	}
	free_context(shell->context);
	shell->context = NULL;
	free_strs(env);
	return (status);
}

bool	is_builtin(char *str)
{
	if (!str)
		return (false);
	if (ft_strncmp(str, "echo", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "pwd", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "unset", -1) == 0)
		return (true);
	return (false);
}
