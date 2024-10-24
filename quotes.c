#include "minishell.h"

char	*snip_snip(char *str, int *i, char *end)
{
	char	*lstr;
	char	*mstr;
	char	*rstr;

	lstr = ft_substr(str, 0, *i);
	mstr = ft_substr(str, *i + 1, end - (str + *i + 1));
	rstr = ft_strdup(end + 1);
	*i = end - str - 2;
	free(str);
	if (lstr == NULL || mstr == NULL || rstr == NULL)
		return (free(lstr), free(rstr), free(mstr), NULL);
	lstr = ft_strappend(ft_strappend(lstr, mstr), rstr);
	free(rstr);
	free(mstr);
	return (lstr);
}

bool	removing_quotes(t_tree *node, char **env)
{
	int		i;
	char	*end;

	i = 0;
	if (node->cmd.type != WORD)
		return (expand_tree(node->left, env) && expand_tree(node->right, env));
	while (node->cmd.str[i])
	{
		end = NULL;
		if (node->cmd.str[i] == '\\')
			i++;
		else if (node->cmd.str[i] == '"')
			end = ft_strchr(&node->cmd.str[i + 1], '"');
		else if (node->cmd.str[i] == '\'')
			end = ft_strchr(&node->cmd.str[i + 1], '\'');
		if (end)
			node->cmd.str = snip_snip(node->cmd.str, &i, end);
		if (node->cmd.str == NULL)
			return (false);
		i++;
	}
	return (true);
}
