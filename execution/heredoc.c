#include "minishell.h"

void	handle_heredoc(t_tree *node, t_context *context)
{
	int		fds[2];
	char	*str;

	if (pipe(fds) == -1)
	{
		ft_putstr_fd("An error has occurred\n", 2);
		context->error = 1;
		return ;
	}
	str = readline("heredoc> ");
	while (str && ft_strncmp(str, node->right->cmd.str, -1) != 0)
	{
		ft_putendl_fd(str, fds[1]);
		free(str);
		str = readline("heredoc> ");
	}
	close(context->input);
	context->input = fds[0];
	close(fds[1]);
}
void	premature_visitation(t_tree *node, t_context *context)
{
	if (node->cmd.type == WORD)
		return ;
	if (node->cmd.type == HEREDOC)
		handle_heredoc(node, context);
	premature_visitation(node->left, context);
	if (node->cmd.type == PIPE)
	{
		context->next = malloc(sizeof(t_context));
		//! HANDLE MALLOC FAIL
		set_context(context->next);
		premature_visitation(node->right, context->next);
	}
}
