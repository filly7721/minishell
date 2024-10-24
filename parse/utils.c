#include "minishell.h"

char	*find_unescaped(char *str, char *symbol)
{
	while (*str != '\0')
	{
		if (*str == '\\')
			str++;
		else if (*str == '"')
			str = ft_strchr(str + 1, '"');
		else if (*str == '\'')
			str = ft_strchr(str + 1, '\'');
		else if (ft_strncmp(str, symbol, ft_strlen(symbol)) == 0)
			return (str);
		str++;
	}
	return (NULL);
}

char	*get_word(char *str)
{
	while (*str == ' ')
		str++;
	while (*str != '\0')
	{
		if (*str == '\\')
			str++;
		else if (*str == '"')
			str = strrchr(str + 1, '"');
		else if (*str == '\'')
			str = strrchr(str + 1, '\'');
		//TODO implement word rules 
		else if (!ft_isalnum(*str) && *str != '$')
			return (str);
		str++;
	}
	return (str);
}

t_tree	*create_node(char *str)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->cmd.str = str;
	node->cmd.type = WORD;
	return (node);
}

void	free_tree(t_tree *head)
{
	if (head == NULL)
		return ;
	free_tree(head->left);
	free_tree(head->right);
	free(head->cmd.str);
	free(head);
}

char	*find_redirect(char *str, t_type *type)
{
	while (*str != '\0')
	{
		if (*str == '\\')
			str++;
		else if (*str == '"')
			str = ft_strchr(str + 1, '"');
		else if (*str == '\'')
			str = ft_strchr(str + 1, '\'');
		else if (ft_strncmp(str, "<<", 2) == 0)
			return (*type = HEREDOC, str);
		else if (ft_strncmp(str, ">>", 2) == 0)
			return (*type = APPEND, str);
		else if (ft_strncmp(str, "<", 1) == 0)
			return (*type = INPUT, str);
		else if (ft_strncmp(str, ">", 1) == 0)
			return (*type = OUTPUT, str);
		str++;
	}
	return (NULL);
}
