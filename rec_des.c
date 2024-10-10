#include "minishell.h"

char	*unescaped(char *str, char c)
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
			str = unescaped(str + 1, '"');
		else if (*str == '\'')
			str = unescaped(str + 1, '\'');
		else if (*str == c)
			return (str);
		str++;
	}
	return (NULL);
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

void	free_null(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

int	split_node(char *str, char *curr, t_tree **left, t_tree **right)
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
	if (!split_node(head->cmd.str, curr, &head->left, &head->right))
		return (write(2, "split node failed\n", 18), 0);
	free(head->cmd.str);
	head->cmd.str = ft_strdup(" ");
	head->cmd.str[0] = symbol;
	head->cmd.type = type;
	return (rec_split(head->left, symbol, type) && rec_split(head->right, symbol, type));
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
			str = unescaped(str + 1, '"');
		else if (*str == '\'')
			str = unescaped(str + 1, '\'');
		else if (!ft_isalpha(*str))
			return (str);
		str++;
	}
	return (str);
}

int	split_word(char *str, char *curr, t_tree **left, t_tree **right)
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
	if (!split_word(head->cmd.str, curr, &head->left, &head->right))
		return (write(2, "split node failed\n", 18), 0);
	free(head->cmd.str);
	head->cmd.str = ft_strdup(" ");
	head->cmd.str[0] = symbol;
	head->cmd.type = type;
	return (word_split(head->left, symbol, type) && word_split(head->right, symbol, type));
}

t_tree	*wrapper(char *str)
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

void print_tree(t_tree *head, int depth)
{
	int	i;

	if (!head)
		return ;
	print_tree(head->left, depth + 1);
	i = 0;
	while (i++ < depth)
		printf("\t");
	printf("(%s)\n", head->cmd.str);
	print_tree(head->right, depth + 1);
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char	*str;

	str = readline("input> ");
	if (!str)
		return 0;
	t_tree *ast;

	ast = wrapper(str);
	print_tree(ast, 0);
	if (execute(ast, env) == false)
		printf("an error happening in execution\n");
}
