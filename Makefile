##
## TEST PERSO
##
## Made by MAMODE Karim at 11:46
##
## Non specific project.
##

NAME=		Snake

BIN_DIR=	bin
SRC_DIR=	src
INC_DIR=	include

SRC=		$(SRC_DIR)/main.c				\
		$(SRC_DIR)/display.c			\
		$(SRC_DIR)/ia_play.c			\
		$(SRC_DIR)/game.c				\

OBJ=		$(SRC:.c=.o)

CC=		gcc

CFLAGS=		-Wall -Wextra -pedantic -ansi -g3 -ggdb -I$(INC_DIR)/
LDFLAGS=	-lncurses -lm

$(NAME):	$(OBJ)
	$(CC) -o $(BIN_DIR)/$(NAME) $(OBJ) $(LDFLAGS)

all:		$(NAME)

clean:
	$(RM) $(OBJ)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:		clean fclean all re
