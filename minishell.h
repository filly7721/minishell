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
# include <dirent.h>
# include <dirent.h>

extern int	g_sig;
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
	char	**strs;
	char	*str;
	t_type	type;
}	t_cmd;

typedef struct s_tree
{
	t_cmd			cmd;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct s_shell
{
	t_list		*env;
	t_tree		*tree;
	t_context	*context;
	int			status;
}	t_shell;

bool		add_history(char *str);

t_tree		*construct_ast(char *str, char **env, t_shell *shell);
char		*find_unescaped(char *str, char *symbol);
t_tree		*create_node(char *str);
bool		split_evenly(char *str, char *curr, t_tree **left, t_tree **right);
bool		split_unevenly(char *str, char *curr, t_tree **left,
				t_tree **right);
char		*get_word(char *str);
char		*find_redirect(char *str, t_type *type);

bool		expand_tree(t_tree *node, char **env, t_shell *shell);

int			execute(t_shell *shell, char **env);
bool		traverse_tree(t_tree *node, char **env, t_context *context);
int			execute_cmd(t_context *context, char **env);

int			execute_builtin(t_shell *context, char **env);
bool		is_builtin(char *str);

bool		handle_pipe(t_tree *node, t_context *context, char **env, t_shell *shell);
bool		handle_heredoc(t_tree *node, t_context *context, char **env, t_shell *shell);
bool		premature_visitation(t_tree *node, t_context *context, char **env, t_shell *shell);

bool		handle_input(t_tree *node, char **env, t_context *context);
bool		handle_output(t_tree *node, char **env,
				t_context *context, bool append);
bool		handle_word(t_tree *node, char **env, t_context *context);

char		*get_path(char *cmd, char **env);
void		free_context(t_context *context);
void		clear_context_list(t_context **context);
t_context	*create_context(void);

void		free_strs(char **strs);
char		*ft_strappend(char *str1, char *str2);
void		free_null(void **ptr);
void		free_tree(t_tree **head);
char		**quote_split(char *str, char c);

bool		trim_tree(t_tree *node, char **env);
bool		removing_quotes(t_tree *node, char **env);

void		clear_shell(t_shell *shell);
t_shell		*create_shell(char **env);
char		**export_env(t_shell *shell);
void		remove_from_env(t_list **head, char *name);

bool		validate_string(char *str);
char		*expanded_str(char *str, char *var, char **env, t_shell *shell);
char		*get_env_value(char *name, char **env, t_shell *shell);
bool		add_env(t_list **list, char *str);
t_list		*find_lst(t_list *list, char *str, int len);
char		*find_and_expand(char *str, char **env, t_shell *shell);
void		onsignint(int signum);
#endif