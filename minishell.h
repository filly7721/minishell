#ifndef MINISHELL_H	
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <memory.h>
# include <fcntl.h>
# include <stdbool.h>
# include "readline/readline.h"
# include "libft.h"
#include <sys/wait.h>

typedef	struct s_context
{
	int		input;
	int		output;
	char	*cmd;
	char	**args;
	bool	is_error;
}	t_context;

typedef enum e_type
{
	SLASH,
	PIPE,
	OR,
	AND,
	INPUT,
	OUTPUT,
	WORD,
}	t_type;

typedef struct s_cmd
{
	char	*str;
	t_type	type;
}	t_cmd;

typedef struct s_tree
{
	t_cmd			cmd;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

int		execute(t_tree *node, char **env);
t_tree	*construct_ast(char *str);

char	*find_unescaped(char *str, char c);
t_tree	*create_node(char *str);
int		split_evenly(char *str, char *curr, t_tree **left, t_tree **right);
int		split_unevenly(char *str, char *curr, t_tree **left, t_tree **right);
char	*get_word(char *str);

void	free_strs(char **strs);
char	*ft_strappend(char *str1, char *str2);
void	free_null(void **ptr);

#endif