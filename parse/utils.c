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

char	*find_unescaped(char *str, char c)
{
	while (*str != '\0')
	{
		if (*str == '\\')
			str++;
		else if (*str == '"')
			str = unescaped_end(str + 1, '"');
		else if (*str == '\'')
			str = unescaped_end(str + 1, '\'');
		else if (*str == c)
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

int	split_evenly(char *str, char *curr, t_tree **left, t_tree **right)
{
	char	*lstr;
	char	*rstr;

	lstr = ft_substr(str, 0, curr - str);
	rstr = ft_substr(str, curr - str + 1, -1);
	if (!lstr || !rstr)
		return (free(lstr), free(rstr), 0);
	*left = create_node(lstr);
	*right = create_node(rstr);
	if (!*left || !*right)
		return (free(lstr), free(rstr), free_null((void **)left), free_null((void **)right), 0);
	return (1);
}

int	split_unevenly(char *str, char *curr, t_tree **left, t_tree **right)
{
	char	*lstr;
	char	*rstr;
	char	*tmp;

	rstr = ft_substr(curr, 1, get_word(curr + 1) - curr - 1);
	if (!rstr)
		return (0);
	// tmp = ft_substr(curr, get_word(curr + 1) - curr, -1);
	tmp = ft_substr(str, 0, curr - str);
	if (!tmp)
		return (free(rstr), 0);
	lstr = ft_strjoin(tmp, curr + ft_strlen(rstr) + 1);
	free(tmp);
	if (!lstr)
		return (free(rstr), 0);
	*left = create_node(lstr);
	*right = create_node(rstr);
	if (!*left || !*right)
		return (free(lstr), free(rstr), free_null((void **)left), free_null((void **)right), 0);
	return (1);
}
