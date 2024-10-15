#include "minishell.h"

char	*unescaped_end(char *str, char c)
{
	while (*str != '\0')
	{
		if (*str == '\\')
			str++;
		else if (*str == c)
			return (str);
		str++;
	}
	return (0);
}

char	*find_unescaped(char *str, char *symbol)
{
	while (*str != '\0')
	{
		if (*str == '\\')
			str++;
		else if (*str == '"')
			str = unescaped_end(str + 1, '"');
		else if (*str == '\'')
			str = unescaped_end(str + 1, '\'');
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
			str = unescaped_end(str + 1, '"');
		else if (*str == '\'')
			str = unescaped_end(str + 1, '\'');
		//TODO implement word rules 
		else if (!ft_isalpha(*str))
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
