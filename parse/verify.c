#include "minishell.h"

/// @brief a therapist
/// @param str someone who needs validation
/// @return you are valid
char	*validate_redirections(char *str)
{
	if (*str == *(str + 1))
		str++;
	str++;
	while (*str)
	{
		if (*str == '\\')
			str++;
		else if (*str != ' ')
			break ;
		str++;
	}
	if (ft_strchr("<>|&", *str) != NULL)
		return (NULL);
	while (*str)
	{
		if (*str == '\\')
			str++;
		else if (*str == '"' || *str == '\'')
			str = ft_strchr(str + 1, *str);
		else if (ft_strchr(" <>|&", *str))
			break ;
		str++;
	}
	return (str);
}

/// @brief a therapist
/// @param str someone who needs validation
/// @return you are valid
char	*validate_pipe(char *str)
{
	str++;
	while (*str)
	{
		if (*str == '\\')
			str++;
		else if (*str != ' ')
			break ;
		str++;
	}
	if (*str == '|' || *str == '\0')
		return (NULL);
	return (str);
}

/// @brief a therapist
/// @param str someone who needs validation
/// @return you are valid
bool	validate_string(char *str)
{
	while (*str)
	{
		if (*str == '\\')
			str++;
		else if (*str == '\'' || *str == '"')
			str = ft_strchr(str + 1, *str);
		else if (*str == '<' || *str == '>')
			str = validate_redirections(str) - 1;
		else if (*str == '|')
			str = validate_pipe(str) - 1;
		if (str == NULL || str == NULL - 1)
			return (false);
		str++;
	}
	return true;
}