// "Hello $SHLVL World"
//  ^           ^
// str         var
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

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
	char *lstr, *rstr, *mstr;
	lstr = ft_strdup(str); // => "Hello "
	mstr = get_env_value(get_word(var), env); // => "1"
	rstr = ft_strdup(var + ft_strlen(get_word(var))); // => "World"
	free(str);
	str = ft_strappend(lstr, mstr)
	// if str fails free all strs ad return NULL
	free(lstr);
	free(mstr);
	str = ft_strappend(str, rstr)
	return 
}

char	*find_and_expand(char *str, char **env)
{
	char	*curr;

	curr = str;
	while (*curr)
	{
		if (*curr == '$')
		{
			*curr = '\0';
			curr++;
			str = expanded_str(str, curr, env);
			curr = str;
		}
		curr++;
	}
}

int main(int ac, char **av, char **env)
{
	char	*str;
	char	*newstr;

	str = ft_strdup(av[1]);
	newstr = find_and_expand(str, env);
	printf("%s\n", newstr);
}
// "Hello "
// "SHLVL World"
