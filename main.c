#include "minishell.h"

char	*get_type(t_type type)
{
	if (type == SLASH)
		return ("SLASH");
	if (type == PIPE)
		return ("PIPE");
	if (type == OR)
		return ("OR");
	if (type == AND)
		return ("AND");
	if (type == INPUT)
		return ("INPUT");
	if (type == OUTPUT)
		return ("OUTPUT");
	if (type == HEREDOC)
		return ("HEREDOC");
	if (type == APPEND)
		return ("APPEND");
	if (type == WORD)
		return ("WORD");
	return ("NONE");
}

void	print_tree(t_tree *head, int depth)
{
	int	i;

	if (!head)
		return ;
	print_tree(head->right, depth + 1);
	i = 0;
	while (i++ < depth)
		printf("\t");
	printf("(%s:%s)\n", get_type(head->cmd.type), head->cmd.str);
	print_tree(head->left, depth + 1);
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_tree	*ast;

	(void)ac;
	(void)av;
	(void)env;
	str = readline("megashell> ");
	if (!str)
		return (1);
	ast = construct_ast(str);
	print_tree(ast, 0);
	printf("exit status is %d\n", execute(ast, env));
}
