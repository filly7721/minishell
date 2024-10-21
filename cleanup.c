#include "minishell.h"

char	*get_env_value(char *name, char **env)
{
	int		i;
	char	*find;

	i = 0;
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

char	*expanded_str(char *str, char *var, char **env)
{
	char	*word;
	char	*lstr;
	char	*rstr; 
	char	*mstr;	

	word = var;
	while (*word == '_' || ft_isalnum(*word))
		word++;
	if (word == var)
		word = ft_strdup("");
	else
		word = ft_substr(var, 0, word - var);
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

char	*find_and_expand(char *str, char **env)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			str[i] = '\0';
			i++;
			str = expanded_str(str, &str[i] , env);
			if (str == NULL)
				return (NULL);
			continue ;
		}
		i++;
	}
	return (str);
}
