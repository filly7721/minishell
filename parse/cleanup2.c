/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:34:46 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 16:36:21 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	flatten_whitespace(char *str)
{
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			str = ft_strchr(str + 1, *str);
		else if (*str > 8 && *str < 13)
			*str = ' ';
		if (str == NULL)
			return ;
		str++;
	}
}

bool	split_args(t_tree *node)
{
	if (node->cmd.type != WORD)
		return (split_args(node->left) && split_args(node->right));
	node->cmd.strs = quote_split(node->cmd.str, ' ');
	if (!node->cmd.strs)
		return (false);
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
	char	**strs;

	if (node->cmd.type != WORD)
		return (removing_quotes(node->left, env)
			&& removing_quotes(node->right, env));
	strs = node->cmd.strs;
	while (*strs)
	{
		i = 0;
		while ((*strs)[i])
		{
			if ((*strs)[i] == '"' || (*strs)[i] == '\'')
				(*strs) = snip_snip((*strs), &i,
						ft_strchr(&((*strs)[i + 1]), (*strs)[i]));
			if ((*strs) == NULL)
				return (false);
			i++;
		}
		strs++;
	}
	return (true);
}
