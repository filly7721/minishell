#include "minishell.h"

char	*unescaped(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\\')
			i++;
		else if (str[i] == (unsigned char)c && str[i - 1] != '\\')
			return ((char *)&str[i]);
		i++;
	}
	return (0);
}

int	push_word(t_list **head, char *str, int *end)
{
	int		i;
	char	*res;
	
	i = 0;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '\\')
			i += 2;
		if (ft_strchr("'\"", str[i]))
		{
			if (str[i] == '"')
				res = unescaped(str + i + 1, '"');
			else if (str[i] == '\'')
				res = unescaped(str + i + 1, '\'');
			if (!res)
				return (0);
			i = res - str;
		}
		i++;
	}
	res = malloc(sizeof(char) * (i + 1));
	ft_strlcpy(res, str, i + 1);
	ft_lstadd_back(head, ft_lstnew(res));
	*end += i;
	return (1);
}

void	ft_putstr(void *str)
{
	ft_putendl_fd(str, 1);
}

t_list	*ft_lstsplit(char *str)
{
	t_list	*head;
	int		i;
	
	i = 0;
	head = NULL;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] && !push_word(&head, str + i, &i))
			return (ft_lstclear(&head, free), NULL);
	}
	return (head);
}

int main()
{
	t_list	*head;
	char	*str;
	char	*new_str;

	str = ft_strdup("");
	while (1)
	{
		new_str = ft_strjoin(str, readline("shelly>"));
		free(str);
		str = new_str;
		if (str[ft_strlen(str) - 1] != '\\')
			break ;
		new_str = ft_substr(str, 0, ft_strlen(str) - 1);
		free(str);
		str = new_str;
	}
	head = ft_lstsplit(str);
	free(str);
	ft_lstiter(head, ft_putstr);
	ft_lstclear(&head, free);
	return (0);
}
