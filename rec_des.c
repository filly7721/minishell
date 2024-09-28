#include "minishell.h"

typedef enum e_type
{
	SLASH,
	PIPE,
	OR,
	AND,
	INPUT,
	OUTPUT,
	WORD,
}	t_type;

typedef struct s_cmd
{
	char	*str;
	t_type	type;
}	t_cmd;

typedef struct s_tree
{
	t_cmd			cmd;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

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

t_tree	*rec_desc(char *str, char symbol)
{
	char	*curr;
	t_tree	*node;
	char	*left;
	char	*right;
	
	if (!str)
		return (NULL);
	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	curr = find_unescaped(str, symbol);
	left = NULL;
	right = NULL;
	node->cmd.type = WORD;
	node->cmd.str = str;
	if (curr)
	{
		left = str;
		right = curr + 1;
		*curr = '\0';
		node->cmd.type = PIPE;
	}
	node->left = rec_desc(left, symbol);
	node->right = rec_desc(right, symbol);
	return (node);
}

// t_tree	*create_node(char *str)
// {
// 	t_tree	*node;

// 	node = malloc(sizeof(t_tree));
// 	if (!node)
// 		return (NULL);
// 	node->left = NULL;
// 	node->right = NULL;
// 	node->cmd.str = NULL;
// 	node->cmd.type = WORD;
// 	return (node);
// }

t_tree	*wrapper(char *str)
{
	t_tree *head;

	head = rec_desc(str, '|');
	return (head);
}

void rec_print(t_tree *head)
{
	if (!head)
		return ;
	rec_print(head->left);
	if (head->cmd.type == PIPE)
		printf("|\n");
	else
		printf("%s\n", head->cmd.str);
	rec_print(head->right);
}

int main()
{
	char	*str;

	str = readline("input>");
	t_tree *ast;

	ast = wrapper(str);
	rec_print(ast);
	free(str);
}