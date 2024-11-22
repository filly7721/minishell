#include "minishell.h"

bool	exit_atoi(char *str, int *num)
{
	int	sign;
	int	i;

	i = 0;
	sign = 1;
	*num = 0;
	while (str[i] == ' ')
		i++;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (ft_isdigit(str[i]))
		*num = ((*num) * 10 + str[i++] - '0') % 256;
	while (str[i] == ' ')
		i++;
	if (str[i])
		return (false);
	if (sign == -1)
		*num = 256 - *num;
	return (true);
}

int	ft_exit(t_shell *shell, char **env)
{
	int	status;

	if (shell->context->args[2])
		return (ft_putstr_fd("megashell: exit: too many arguments\n", 2), 1);
	free_strs(env);
	if (!shell->context->args[1])
		(clear_shell(shell), ft_putendl_fd("exit", 2), exit(0));
	if (exit_atoi(shell->context->args[1], &status) == false)
	{
		ft_putstr_fd("megashell: ", 2);
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(shell->context->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		status = 255;
	}
	clear_shell(shell);
	exit(status % 256);
}

int	ft_env(t_shell *shell, char **env)
{
	int		fd;

	fd = shell->context->output;
	if (fd == -1)
		fd = 1;
	while (*env)
		ft_putendl_fd(*env++, fd);
	return (0);
}

char	*get_pwd(void)
{
	char	*buff;
	size_t	size;

	size = 32;
	while (1)
	{
		buff = malloc(size);
		if (!buff)
			return (NULL);
		if (getcwd(buff, size) != NULL)
			break ;
		free(buff);
		size *= 2;
	}
	return (buff);
}

void	print_cd_error(char *directory)
{
	ft_putstr_fd(directory, 2);
	if (errno == ENOTDIR)
		ft_putstr_fd(": Not a directory\n", 2);
	else if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (errno == ELOOP)
		ft_putstr_fd(": Too many levels of symbolic links\n", 2);
	else if (errno == ENAMETOOLONG)
		ft_putstr_fd(": File name too long\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
}

bool	update_pwd(t_shell *shell, char *pwd)
{
	char	*env_str;

	env_str = ft_strjoin("OLDPWD=", pwd);
	if (!env_str)
		return (false);
	if (!add_env(&shell->env, env_str))
		return (free(env_str), false);
	free(env_str);
	pwd = get_pwd();
	if (!pwd)
		return (false);
	env_str = ft_strjoin("PWD=", pwd);
	if (!env_str)
		return (false);
	if (!add_env(&shell->env, env_str))
		return (free(env_str), false);
	free(env_str);
	free(pwd);
	return (true);
}

int	ft_cd(t_shell *shell, char **env)
{
	char	*directory;
	char	*old_pwd;

	if (shell->context->args[1] == NULL)
	{
		directory = get_env_value("HOME", env, shell);
		if (!directory || directory[0] == '\0')
			return (free(directory), ft_putstr_fd("cd: HOME not set", 2), 1);
	}
	else
		directory = ft_strdup(shell->context->args[1]);
	old_pwd = get_pwd();
	if (!old_pwd)
		return (ft_putstr_fd("An error has occurred\n", 2), 1);
	if (chdir(directory) != 0)
		return (print_cd_error(directory), free(old_pwd), free(directory), 1);
	free(directory);
	if (!update_pwd(shell, old_pwd))
		return (ft_putstr_fd("An error has occurred\n", 2), free(old_pwd), 1);
	free(old_pwd);
	return (0);
}

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
	char	*str;

	fd = context->output;
	if (fd == -1)
		fd = 1;
	str = get_pwd();
	if (!str)
		return (ft_putstr_fd("An error has occurred\n", 2), 1);
	ft_putendl_fd(str, 2);
	free(str);
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
			return (ft_putstr_fd("An error has occurred\n", 2), 1);
		remove_from_env(&shell->env, name);
		free(name);
		i++;
	}
	return (0);
}

bool	validate_export(char *str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	while (*str != '\0' && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}

void	print_export(t_list *list, int fd)
{
	char	*end;
	t_list	*curr;

	curr = list;
	while (curr)
	{
		end = ft_strchr(curr->content, '=') + 1;
		ft_putstr_fd("declare -x ", fd);
		write(fd, curr->content, end - (char *)curr->content);
		ft_putchar_fd('"', fd);
		ft_putstr_fd(end, fd);
		ft_putstr_fd("\"\n", fd);
		curr = curr->next;
	}
}

int	sort_print(t_list *list, int fd)
{
	int		count;
	char	*temp;
	t_list	*curr;

	if (!list)
		return (ft_putstr_fd("An error has occurred\n", 2), 1);
	count = ft_lstsize(list);
	while (count)
	{
		curr = list;
		while (curr->next)
		{
			if (ft_strncmp(curr->content, curr->next->content, -1) > 0)
			{
				temp = curr->content;
				curr->content = curr->next->content;
				curr->next->content = temp;
			}
			curr = curr->next;
		}
		count--;
	}
	print_export(list, fd);
	return (ft_lstclear(&list, free), 0);
}

int	ft_export(t_shell *shell)
{
	int		i;
	int		fd;

	fd = shell->context->output;
	if (fd == -1)
		fd = 1;
	i = 1;
	while (shell->context->args[i])
	{
		if (!validate_export(shell->context->args[i]))
			return (ft_putstr_fd("Invalid variable name\n", 2), 1);
		if (!add_env(&shell->env, shell->context->args[i]))
			return (ft_putstr_fd("An error has occurred\n", 2), 1);
		i++;
	}
	if (shell->context->args[1] == NULL)
		return (sort_print(ft_lstmap(shell->env,
					(void *(*)(void *))ft_strdup, free), fd));
	return (0);
}

int	execute_builtin(t_shell *shell, char **env)
{
	int	status;

	clear_context_list(&shell->context->next);
	status = 1;
	if (ft_strncmp(shell->context->cmd, "echo", -1) == 0)
		status = ft_echo(shell->context);
	else if (ft_strncmp(shell->context->cmd, "pwd", -1) == 0)
		status = ft_pwd(shell->context);
	else if (ft_strncmp(shell->context->cmd, "unset", -1) == 0)
		status = ft_unset(shell);
	else if (ft_strncmp(shell->context->cmd, "export", -1) == 0)
		status = ft_export(shell);
	else if (ft_strncmp(shell->context->cmd, "cd", -1) == 0)
		status = ft_cd(shell, env);
	else if (ft_strncmp(shell->context->cmd, "env", -1) == 0)
		status = ft_env(shell, env);
	else if (ft_strncmp(shell->context->cmd, "exit", -1) == 0)
		status = ft_exit(shell, env);
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
	else if (ft_strncmp(str, "export", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "cd", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "env", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "exit", -1) == 0)
		return (true);
	return (false);
}
