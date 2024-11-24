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

char	*get_input(void)
{
	char	*str;
	int		i;
	
	while (1)
	{
		i = 0;
		if (isatty(0))
			str = readline("megashell> ");
		else
			str = readline("");
		if (!str)
			return (NULL);
		flatten_whitespace(str);
		while (str[i] == ' ')
			i++;
		if (str[i] == '\0')
		{
			free(str);
			continue ;
		}
		if ((add_history(str), true) && validate_string(str))
			return (str);
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
	if (!isatty(0))
		rl_outstream = stdin;
	while (1)
	{
		signal(SIGINT, onsignint);
		signal(SIGQUIT, SIG_IGN);
		new_env = export_env(shell);
		if (!new_env)
			return (clear_shell(shell), ft_putstr_fd("env failed`\n", 2), 1);
		str = get_input();
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
		shell->status = execute(shell, new_env);
	}
	free_strs(new_env);
	g_sig = shell->status;
	clear_shell(shell);
	return (g_sig);
}
