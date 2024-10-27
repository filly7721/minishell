#include "minishell.h"

char	**export_env(t_shell *shell)
{
	t_list	*curr;
	char	**env;
	int		i;

	curr = shell->env;
	env = ft_calloc(sizeof(char *), ft_lstsize(curr) + 1);
	if (!env)
		return (NULL);
	i = 0;
	while (curr)
	{
		env[i] = ft_strdup((char *)(curr->content));
		if (!env[i])
			return (free_strs(env), NULL);
		curr = curr->next;
		i++;
	}
	return (env);
}

bool	init_shell(t_shell *shell, char **env)
{
	t_list	*node;
	char	*str;

	shell->context = NULL;
	shell->env = NULL;
	shell->tree = NULL;
	while (*env)
	{
		str = ft_strdup(*env);
		if (!str)
			return (ft_lstclear(&shell->env, free), false);
		node = ft_lstnew(str);
		if (!node)
			return (free(str), ft_lstclear(&shell->env, free), false);
		ft_lstadd_back(&shell->env, node);
		env++;
	}
	return (true);
}

void	clear_shell(t_shell *shell)
{
	clear_context_list(shell->context);
	ft_lstclear(&shell->env, free);
	free_tree(shell->tree);
}
