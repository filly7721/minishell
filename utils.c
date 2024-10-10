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