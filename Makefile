PARSE_SRCS	=	utils.c rec_des.c cleanup.c verify.c cleanup2.c

EXEC_SRCS	=	execution.c utils.c handler.c heredoc.c utils2.c
EXEC_SRCS	+=	${addprefix builtins/, ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_export.c ft_pwd.c ft_unset.c builtins.c}

SRCS		=	utils.c main.c env.c utils2.c env_utils.c
SRCS		+=	${addprefix parse/, ${PARSE_SRCS}} ${addprefix execution/, ${EXEC_SRCS}}

OBJS		=	${SRCS:.c=.o}

LIBFT		=	libft/libft.a

READLINE	=	-L/opt/vagrant/embedded/lib/ -lreadline 

CFLAGS		=	-Wall -Wextra -Werror
CFLAGS		+=	-Ilibft -I./ -I/opt/vagrant/embedded/include/readline -I/opt/vagrant/embedded/include

NAME		=	minishell

all			:	${NAME}

%.o			:	%.c
		cc ${CFLAGS} -c $< -o $@

${LIBFT}	:
		make -C libft

${NAME}		:	${LIBFT} ${OBJS}
		cc ${CFLAGS} ${OBJS} ${READLINE} ${LIBFT} -o ${NAME}

clean		:
		make -C libft clean
		rm -f ${OBJS}

fclean		:	clean
		rm -f ${LIBFT}
		rm -f ${NAME}

re			: fclean all

.PHONY		:	all clean fclean re