/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssiddiqu <ssiddiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:32:50 by ssiddiqu          #+#    #+#             */
/*   Updated: 2024/10/20 21:15:17 by ssiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// "Hello $SHLVL World"
//  ^           ^
// str         var
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"


char *getting_word(char *str)
{
	int	i;
	
	i = 0;
	while (str[i] == '_' || ft_isalnum(str[i]))
		i++;
	return (ft_substr(str, 0, i - 1));
}

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

char	*ft_strappend(char *str1, char *str2)
{
	char	*res;

	if (str1 == NULL || str2 == NULL)
		return (NULL);
	res = ft_strjoin(str1, str2);
	free(str1);
	return (res);
}

char	*expanded_str(char *str, char *var, char **env)
{
	char	*word;
	char	*lstr;
	char	*rstr; 
	char	*mstr;	

	word = getting_word(var);
	if (word == NULL)
		return (free(str), NULL);
	lstr = ft_strdup(str); // => "Hello "
	mstr = get_env_value(word, env); // => "1"
	rstr = ft_strdup(var + ft_strlen(word)); // => "World"
	free(word);
	free(str);
	if (lstr == NULL || mstr == NULL || rstr == NULL)
		return (free(lstr), free(mstr), free(rstr), NULL);
	lstr = ft_strappend(lstr, mstr);
	lstr = ft_strappend(lstr, rstr);
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
			continue;
		}
		i++;
	}
	return (str);
}

int main(int ac, char **av, char **env)
{
	char	*str;
	char	*newstr;

	str = ft_strdup(av[1]);
	newstr = find_and_expand(str, env);
	printf("%s\n", newstr);
	free(newstr);
}
// "Hello "
// "SHLVL World"
