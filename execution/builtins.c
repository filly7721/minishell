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

bool	add_env(t_list **list, char *str)
{
	char	*copy;
	t_list	*temp;

	if (ft_strchr(str, '=') != NULL)
		copy = ft_strdup(str);
	else
		copy = ft_strjoin(str, "=");
	if (!copy)
		return (ft_putstr_fd("An error has occured\n", 2), 1);
	temp = ft_lstnew(copy);
	if (!temp)
		return (ft_putstr_fd("An error has occured\n", 2), free(copy), 1);
	ft_lstadd_front(list, temp);
	return (true);
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
			return (ft_putstr_fd("An error has occured\n", 2), 1);
		i++;
	}
	if (shell->context->args[1] == NULL)
		return (sort_print(ft_lstmap(shell->env, (void *(*)(void *))ft_strdup, free), fd));
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
	else if (ft_strncmp(shell->context->cmd, "export", -1) == 0)
		status = ft_export(shell);
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
	else if (ft_strncmp(str, "export", -1) == 0)
		return (true);
	return (false);
}
