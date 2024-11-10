#include "minishell.h"

int	g_sig = 0;

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
	int		i;
	char	**strs;

	if (!head)
		return ;
	print_tree(head->right, depth + 1);
	i = 0;
	while (i++ < depth)
		printf("\t");
	if (head->cmd.type == WORD)
	{
		printf("[");
		strs = head->cmd.strs;
		while (*strs)
			printf("%s, ", *strs++);
		printf("]\n");
	}
	else
		printf("(%s:%s)\n", get_type(head->cmd.type), head->cmd.str);
	print_tree(head->left, depth + 1);
}

char	*get_input(void)
{
	char	*str;
	
	while (1)
	{
		str = readline("megashell> ");
		if (!str)
			return (NULL);
		if (validate_string(str))
			return (str); // TODO add history
		ft_putstr_fd("parse error\n", 2);
		free(str);
	}
}

void onsignint(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_shell	*shell;
	char	**new_env;

	(void)ac;
	(void)av;
	shell = create_shell(env);
	if (!shell)
		return (1);
	while (1)
	{
		signal(SIGINT, onsignint);
		signal(SIGQUIT, SIG_IGN);
		new_env = export_env(shell);
		if (!new_env)
			return (clear_shell(shell), ft_putstr_fd("env failed`\n", 2), 1);
		str = get_input();
		add_history(str);
		if (!str)
			break ;
		if (g_sig != 0)
		{
			g_sig = 0;
			shell->status = 1;
		}
		shell->tree = construct_ast(str, new_env, shell);
		if (!shell->tree)
			return (clear_shell(shell), ft_putstr_fd("Ast failed`\n", 2), 1);
		print_tree(shell->tree, 0);
		shell->status = execute(shell, new_env);
	}
	free_strs(new_env);
	clear_shell(shell);
	return (shell->status);
}
