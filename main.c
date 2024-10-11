#include "minishell.h"

void	print_tree(t_tree *head, int depth)
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

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char	*str;

	str = readline("input> ");
	if (!str)
		return 0;
	t_tree *ast;

	ast = construct_ast(str);
	print_tree(ast, 0);
	if (execute(ast, env) == false)
		printf("an error happening in execution\n");
	while (wait(NULL) != -1)
		;
}
