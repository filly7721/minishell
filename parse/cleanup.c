/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:19 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 16:36:15 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *name, char **env, t_shell *shell)
{
	int		i;
	char	*find;

	i = 0;
	if (strncmp(name, "?", 2) == 0)
		return (ft_itoa(shell->status));
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
	if (ft_isdigit(*str))
	{
		word = ft_strdup(" ");
		if (word)
			word[0] = *str;
		return (word);
	}
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

char	*expanded_str(char *str, char *var, char **env, t_shell *shell)
{
	char	*word;
	char	*lstr;
	char	*rstr;
	char	*mstr;	

	word = get_var_name(var);
	if (word == NULL)
		return (free(str), NULL);
	lstr = ft_strdup(str);
	mstr = get_env_value(word, env, shell);
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

bool	expand_tree(t_tree *n, char **env, t_shell *shell)
{
	int		i;
	bool	dquote;

	if (n->cmd.type == HEREDOC)
		return (expand_tree(n->left, env, shell));
	if (n->cmd.type != WORD)
		return (expand_tree(n->left, env, shell)
			&& expand_tree(n->right, env, shell));
	i = -1;
	dquote = false;
	while (n->cmd.str && n->cmd.str[++i])
	{
		if (n->cmd.str[i] == '"')
			dquote = !dquote;
		else if (n->cmd.str[i] == '\'' && dquote == false)
			i = ft_strchr(n->cmd.str + i + 1, '\'') - n->cmd.str;
		else if (n->cmd.str[i] == '$')
		{
			n->cmd.str[i++] = '\0';
			n->cmd.str = expanded_str(n->cmd.str, &n->cmd.str[i--], env, shell);
			i--;
		}
	}
	return (n->cmd.str != NULL);
}
