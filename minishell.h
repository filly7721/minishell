#ifndef MINISHELL_H	
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <memory.h>
# include <fcntl.h>
# include <stdbool.h>
# include "readline/readline.h"
# include "libft.h"
# include <sys/wait.h>
# include <errno.h>

typedef struct s_context
{
	int					input;
	int					output;
	char				*cmd;
	char				**args;
	int					error;
	struct s_context	*next;
}	t_context;

typedef enum e_type
{
	SLASH,
	PIPE,
	OR,
	AND,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
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

t_tree	*construct_ast(char *str);
char	*find_unescaped(char *str, char *symbol);
t_tree	*create_node(char *str);
bool	split_evenly(char *str, char *curr, t_tree **left, t_tree **right);
bool	split_unevenly(char *str, char *curr, t_tree **left, t_tree **right);
char	*get_word(char *str);
char	*find_redirect(char *str, t_type *type);

int		execute(t_tree *node, char **env);
bool	traverse_tree(t_tree *node, char **env, t_context *context);
void	execute_cmd(t_context *context, char **env);

bool	handle_input(t_tree *node, char **env, t_context *context);
bool	handle_output(t_tree *node, char **env, t_context *context);
bool	handle_pipe(t_tree *node, char **env, t_context *context);
bool	handle_word(t_tree *node, char **env, t_context *context);

char	*get_path(char *cmd, char **env);
void	reset_context(t_context *context);
void	clear_context_list(t_context *context);
void	set_context(t_context *context);

void	free_strs(char **strs);
char	*ft_strappend(char *str1, char *str2);
void	free_null(void **ptr);
void	free_tree(t_tree *head);

#endif