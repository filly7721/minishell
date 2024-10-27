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

bool	trim_tree(t_tree *node, char **env)
{
	char	*tmp;

	if (node->cmd.type != WORD)
		return (trim_tree(node->left, env) && trim_tree(node->right, env));
	tmp = node->cmd.str;
	node->cmd.str = ft_strtrim(node->cmd.str, " ");
	free(tmp);
	return (node->cmd.str != NULL);
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_shell	shell;

	(void)ac;
	(void)av;
	if (!init_shell(&shell, env))
		return (1);
	str = readline("megashell> ");
	if (!str)
		return (1);
	shell.tree = construct_ast(str);
	if (!shell.tree)
		return (1);
	if (!expand_tree(shell.tree, env) || !trim_tree(shell.tree, env)
		|| !removing_quotes(shell.tree, env))
		//TODO FREE
		return (ft_putstr_fd("Cleanup failed\n", 2), 1);
	print_tree(shell.tree, 0);
	printf("exit status is %d\n", execute(&shell));
	clear_shell(&shell);
}
