#include "minishell.h"

char	*get_prompt(char state)
{
	if (state == '"')
		return ("DQUOTE>");
	if (state == '\'')
		return ("SQUOTE>");
	return ("Shelly>");
}

int	push_word(t_list **head, char *str, int *end)
{
	int		i;
	char	*res;
	
	i = 0;
	while (str[i] && str[i] != ' ')
	{
		while (str[i] && !ft_strchr(" \"'", str[i]))
			i++;
		if (str[i] == '"')
			res = ft_strchr(str + i + 1, '"');
		else if (str[i] == '\'')
			res = ft_strchr(str + i + 1, '\'');
		else
			continue ;
		if (!res)
			return (0);
		i = res - str + 1;
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
		while (str[i] && str[i] != ' ')
			i++;
	}
	return (head);
}
t_list *ft_qsplit(char *str)
{

}

int main()
{
	char	*str;
	// char	*new_str;
	// char	state;

	// str = ft_strdup("");
	// state = '\\';
	// while (state != ' ')
	// {
	// 	new_str = ft_strjoin(str, readline(get_prompt(state)));
	// 	free(str);
	// 	state = get_state(new_str);
	// 	str = new_str;
	// }

	str = readline("input>");
	t_list	*head;

	head = ft_lstsplit(str);
	ft_lstiter(head, ft_putstr);
	ft_lstclear(&head, free);
	free(str);
}