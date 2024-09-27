SRCS		=	parse.c

OBJS		=	${SRCS:.c=.o}

LIBFT		=	libft/libft.a

READLINE	=	-L/opt/vagrant/embedded/lib/ -lreadline 

CFLAGS		=	-Wall -Wextra -Werror -Ilibft -g3 -I/opt/vagrant/embedded/include/readline -I/opt/vagrant/embedded/include

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