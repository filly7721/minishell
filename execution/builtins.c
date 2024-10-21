#include "minishell.h"

int	ft_echo(t_context *context)
{
	char	**strs;
	
	strs = context->args + 1;
	if (ft_strncmp(context->args[1], "-n", -1) == 0)
		strs++;
	ft_putstr_fd(*strs++, context->output);
	while (*strs)
	{
		ft_putchar_fd(' ', context->output);
		ft_putstr_fd(*strs++, context->output);
	}
	if (ft_strncmp(context->args[1], "-n", -1) == 0)
		ft_putchar_fd(' ', context->output);
	return (0);
}

int	ft_pwd(t_context *context, char **env)
{
	while (*env && ft_strncmp("pwd=", *env, 4) != 0)
		env++;
	if (*env == NULL)
		ft_putendl_fd("", context->output);
	else
		ft_putendl_fd(*env + 4, context->output);
	return (0);
}

int	execute_builtin(t_context *context, char **env)
{
	int	status;

	clear_context_list(context->next);
	ft_putstr_fd("eyo builtin time\n", 2);
	if (ft_strncmp(context->cmd, "echo", -1) == 0)
		status = ft_echo(context);
	else if (ft_strncmp(context->cmd, "pwd", -1) == 0)
		status = ft_pwd(context, env);
	else 
	{
		ft_putstr_fd("unhandled builtin", 2);
		status = 1;
	}
	free_context(context);
	return (status);
}

bool	is_builtin(char *str)
{
	if (ft_strncmp(str, "echo", -1) == 0)
		return (true);
	else if (ft_strncmp(str, "pwd", -1) == 0)
		return (true);
	return (false);
}
