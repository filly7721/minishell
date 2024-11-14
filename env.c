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

t_shell	*create_shell(char **env)
{
	t_shell	*shell;
	t_list	*node;
	char	*str;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->context = NULL;
	shell->env = NULL;
	shell->tree = NULL;
	while (*env)
	{
		str = ft_strdup(*env);
		if (!str)
			return (ft_lstclear(&shell->env, free), free(shell), NULL);
		node = ft_lstnew(str);
		if (!node)
			return (free(str), ft_lstclear(&shell->env, free),
				free(shell), NULL);
		ft_lstadd_back(&shell->env, node);
		env++;
	}
	shell->status = 0;
	return (shell);
}

void	clear_shell(t_shell *shell)
{
	clear_context_list(&shell->context);
	ft_lstclear(&shell->env, free);
	free_tree(shell->tree);
	free(shell);
}

void	remove_from_env(t_list **head, char *name)
{
	t_list	*tmp;
	
	if (*head == NULL)
		return ;
	if (ft_strncmp((*head)->content, name, ft_strlen(name)) == 0)
	{
		tmp = (*head)->next;
		ft_lstdelone(*head, free);
		*head = tmp;
		remove_from_env(head, name);
		return ;
	}
	remove_from_env(&((*head)->next), name);
}
