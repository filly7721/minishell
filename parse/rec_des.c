#include "minishell.h"

int	rec_split(t_tree *head, char symbol, t_type type)
{
	char	*curr;

	if (head == NULL)
		return (1);
	if (head->cmd.type != WORD)
		return (rec_split(head->left, symbol, type) && rec_split(head->right, symbol, type));
	curr = find_unescaped(head->cmd.str, symbol);
	if (!curr)
		return (1);
	if (!split_evenly(head->cmd.str, curr, &head->left, &head->right))
		return (write(2, "split node failed\n", 18), 0);
	free(head->cmd.str);
	head->cmd.str = ft_strdup(" ");
	head->cmd.str[0] = symbol;
	head->cmd.type = type;
	return (rec_split(head->left, symbol, type) && rec_split(head->right, symbol, type));
}

int	word_split(t_tree *head, char symbol, t_type type)
{
	char	*curr;

	if (head == NULL)
		return (1);
	if (head->cmd.type != WORD)
		return (word_split(head->left, symbol, type) && word_split(head->right, symbol, type));
	curr = find_unescaped(head->cmd.str, symbol);
	if (!curr)
		return (1);
	if (!split_unevenly(head->cmd.str, curr, &head->left, &head->right))
		return (write(2, "split node failed\n", 18), 0);
	free(head->cmd.str);
	head->cmd.str = ft_strdup(" ");
	head->cmd.str[0] = symbol;
	head->cmd.type = type;
	return (word_split(head->left, symbol, type) && word_split(head->right, symbol, type));
}

t_tree	*construct_ast(char *str)
{
	t_tree *head;

	head = create_node(str);
	if (!head)
		return (NULL);
	if (!rec_split(head, '|', PIPE))
		return (write(2, "rec split failed\n", 17), NULL);
	if (!word_split(head, '<', INPUT))
		return (write(2, "rec split failed\n", 17), NULL);
	if (!word_split(head, '>', OUTPUT))
		return (write(2, "rec split failed\n", 17), NULL);
	return (head);
}
