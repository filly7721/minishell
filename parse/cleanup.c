#include "minishell.h"

char	*get_env_value(char *name, char **env)
{
	int		i;
	char	*find;

	i = 0;
	if (strncmp(name, "?", 2) == 0)
		return (ft_itoa(g_status));
	find = ft_strjoin(name, "=");
	if (find == NULL)
		return (NULL);
	while (env[i] && ft_strncmp(env[i], find, ft_strlen(find)) != 0)
		i++;
	free(find);
	if (!env[i])
		return (ft_strdup(""));
	return (ft_strdup(env[i] + ft_strlen(name) + 1));
}

char	*get_var_name(char *str)
{
	char	*word;

	word = str;
	if (*str == '?')
		return (ft_strdup("?"));
	while (*word == '_' || ft_isalnum(*word))
		word++;
	if (word == str)
		word = ft_strdup("");
	else
		word = ft_substr(str, 0, word - str);
	return (word);
}

char	*expanded_str(char *str, char *var, char **env)
{
	char	*word;
	char	*lstr;
	char	*rstr;
	char	*mstr;	

	word = get_var_name(var);
	if (word == NULL)
		return (free(str), NULL);
	lstr = ft_strdup(str);
	mstr = get_env_value(word, env);
	rstr = ft_strdup(var + ft_strlen(word));
	free(word);
	free(str);
	if (lstr == NULL || mstr == NULL || rstr == NULL)
		return (free(lstr), free(mstr), free(rstr), NULL);
	lstr = ft_strappend(ft_strappend(lstr, mstr), rstr);
	free(rstr);
	free(mstr);
	return (lstr);
}

bool	expand_tree(t_tree *node, char **env)
{
	int	i;

	if (node->cmd.type == HEREDOC)
		return (expand_tree(node->left, env));
	if (node->cmd.type != WORD)
		return (expand_tree(node->left, env) && expand_tree(node->right, env));
	i = 0;
	while (node->cmd.str[i])
	{
		if (node->cmd.str[i] == '$')
		{
			node->cmd.str[i] = '\0';
			i++;
			node->cmd.str
				= expanded_str(node->cmd.str, &node->cmd.str[i], env);
			if (node->cmd.str == NULL)
				return (false);
			continue ;
		}
		i++;
	}
	return (true);
}

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
		return (removing_quotes(node->left, env)
			&& removing_quotes(node->right, env));
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