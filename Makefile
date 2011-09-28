##
## TEST PERSO
##
## Made by MAMODE Karim at 11:46
##
## Non specific project.
##

NAME=		Snake

SRC=		main.c				\
		display.c			\
		ai_play.c			\
		game.c				\

OBJ=		$(SRC:.c=.o)

CC=		gcc

CFLAGS=		-Wall -Wextra -pedantic -ansi -g3 -ggdb
LDFLAGS=	-lncurses -lm

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

all:		$(NAME)

clean:
	$(RM) $(OBJ)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:		clean fclean all re
