/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:18:11 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/11/25 15:18:12 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_unescaped(char *str, char *symbol)
{
	while (*str != '\0')
	{
		if (*str == '"')
			str = ft_strchr(str + 1, '"');
		else if (*str == '\'')
			str = ft_strchr(str + 1, '\'');
		else if (ft_strncmp(str, symbol, ft_strlen(symbol)) == 0)
			return (str);
		str++;
	}
	return (NULL);
}

char	*get_word(char *str)
{
	while (*str == ' ')
		str++;
	while (*str != '\0')
	{
		if (*str == '"')
			str = ft_strchr(str + 1, '"');
		else if (*str == '\'')
			str = ft_strchr(str + 1, '\'');
		else if (ft_strchr(" |<>\t", *str) != NULL)
			return (str);
		str++;
	}
	return (str);
}

t_tree	*create_node(char *str)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->cmd.str = str;
	node->cmd.strs = NULL;
	node->cmd.type = WORD;
	return (node);
}

void	free_tree(t_tree **head)
{
	if (*head == NULL)
		return ;
	free_tree(&(*head)->left);
	free_tree(&(*head)->right);
	free((*head)->cmd.str);
	free_strs((*head)->cmd.strs);
	free(*head);
	*head = NULL;
}

char	*find_redirect(char *str, t_type *type)
{
	while (*str != '\0')
	{
		if (*str == '"')
			str = ft_strchr(str + 1, '"');
		else if (*str == '\'')
			str = ft_strchr(str + 1, '\'');
		else if (ft_strncmp(str, "<<", 2) == 0)
			*type = HEREDOC;
		else if (ft_strncmp(str, ">>", 2) == 0)
			*type = APPEND;
		else if (ft_strncmp(str, "<", 1) == 0)
			*type = INPUT;
		else if (ft_strncmp(str, ">", 1) == 0)
			*type = OUTPUT;
		if (*type != WORD)
			return (str);
		str++;
	}
	return (NULL);
}
