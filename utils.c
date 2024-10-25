#include "minishell.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	if (strs == NULL)
		return ;
	while (strs[i] != NULL)
		free(strs[i++]);
	free(strs);
}

char	*ft_strappend(char *str1, char *str2)
{
	char	*res;

	if (str1 == NULL || str2 == NULL)
		return (NULL);
	res = ft_strjoin(str1, str2);
	free(str1);
	return (res);
}

void	free_null(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

int	count_words(char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			count++;
		while (*str != '\0' && *str != c)
		{
			if (*str == '"' || *str == '\'')
				str = ft_strchr(str + 1, *str);
			str++;
		}
	}
	return (count);
}

char	*extract_word(char *str, char c)
{
	char	*curr;
	int		i;
	
	curr = str;
	while (*curr != '\0' && *curr != c)
	{
		if (*curr == '"' || *curr == '\'')
			curr = ft_strchr(curr + 1, *curr);
		curr++;
	}
	return (ft_substr(str, 0, curr - str));
}

char	**quote_split(char *str, char c)
{
	char	**res;
	int		i;

	res = ft_calloc(count_words(str, c) + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			res[i++] = extract_word(str, c);
		if (res[i - 1] == NULL)
			return (free_strs(res), NULL);
		while (*str != '\0' && *str != c)
		{
			if (*str == '"' || *str == '\'')
				str = ft_strchr(str + 1, *str);
			str++;
		}
	}
	return (res);
}
