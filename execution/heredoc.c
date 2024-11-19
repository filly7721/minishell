#include "minishell.h"

char	*find_and_expand(char *str, char **env, t_shell *shell)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			str[i++] = '\0';
			str = expanded_str(str, str + i--, env, shell);
			if (!str)
				return (NULL);
			continue ;
		}
		i++;
	}
	return (str);
}

bool	handle_heredoc(t_tree *node, t_context *context, char **env, t_shell *shell)
{
	int		fds[2];
	char	*str;

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	str = readline("heredoc> ");
	while (str && ft_strncmp(str, node->right->cmd.strs[0], -1) != 0)
	{
		str = find_and_expand(str, env, shell);
		ft_putendl_fd(str, fds[1]);
		free(str);
		str = readline("heredoc> ");
	}
	close(context->input);
	context->input = fds[0];
	close(fds[1]);
	return (str != NULL);
}

bool	handle_pipe(t_tree *node, t_context *context, char **env, t_shell *shell)
{
	int	fds[2];

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	context->next = create_context();
	if (!context->next)
	{
		close(fds[0]);
		close(fds[1]);
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return (false);
	}
	context->output = fds[1];
	context->next->input = fds[0];
	return (premature_visitation(node->right, context->next, env, shell));
}

bool	premature_visitation(t_tree *node, t_context *context, char **env, t_shell *shell)
{
	if (node->cmd.type == WORD)
		return (true);
	if (node->cmd.type == HEREDOC)
		if (!handle_heredoc(node, context, env, shell))
			return (false);
	if (!premature_visitation(node->left, context, env, shell))
		return (false);
	if (node->cmd.type == PIPE)
		if (!handle_pipe(node, context, env, shell))
			return (false);
	return (true);
}
