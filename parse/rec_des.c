#include "minishell.h"

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
		return (free(lstr), free(rstr),
			free_null((void **)left), free_null((void **)right), 0);
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
		return (free(lstr), free(rstr),
			free_null((void **)left), free_null((void **)right), 0);
	return (1);
}

int	rec_split(t_tree *head, char *symbol, t_type type)
{
	char	*curr;

	if (head == NULL)
		return (1);
	if (head->cmd.type != WORD)
		return (rec_split(head->left, symbol, type)
			&& rec_split(head->right, symbol, type));
	curr = find_unescaped(head->cmd.str, symbol);
	if (!curr)
		return (1);
	if (!split_evenly(head->cmd.str, curr, &head->left, &head->right))
		return (ft_putstr_fd("split node failed\n", 2), 0);
	free(head->cmd.str);
	head->cmd.str = ft_strdup(symbol);
	head->cmd.type = type;
	return (rec_split(head->left, symbol, type)
		&& rec_split(head->right, symbol, type));
}

int	word_split(t_tree *head, char *symbol, t_type type)
{
	char	*curr;

	if (head == NULL)
		return (1);
	if (head->cmd.type != WORD)
		return (word_split(head->left, symbol, type)
			&& word_split(head->right, symbol, type));
	curr = find_unescaped(head->cmd.str, symbol);
	if (!curr)
		return (1);
	if (!split_unevenly(head->cmd.str, curr, &head->left, &head->right))
		return (ft_putstr_fd("split node failed\n", 2), 0);
	free(head->cmd.str);
	head->cmd.str = ft_strdup(symbol);
	head->cmd.type = type;
	return (word_split(head->left, symbol, type)
		&& word_split(head->right, symbol, type));
}

t_tree	*construct_ast(char *str)
{
	t_tree	*head;

	head = create_node(str);
	if (!head)
		return (NULL);
	if (!rec_split(head, "|", PIPE))
		return (write(2, "rec split failed\n", 17), NULL);
	if (!word_split(head, "<", INPUT))
		return (write(2, "rec split failed\n", 17), NULL);
	if (!word_split(head, ">", OUTPUT))
		return (write(2, "rec split failed\n", 17), NULL);
	return (head);
}
