#ifndef MINISHELL_H	
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <memory.h>
# include <fcntl.h>
# include <stdbool.h>
# include "readline/readline.h"
# include "libft.h"
// # include "sys/wait.h"

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


bool	execute(t_tree *node, char **env);

#endif