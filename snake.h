/*
 * Snake Game
 * Made By MAMODE Karim
 * Created the 22th of September 2011 at 15:00
 * Last Modification: 
 *
 * HEADER
 */

#ifndef	SNAKE_H
#define	SNAKE_H

/*
 * Option available
 */

# define OPT_IA     1
# define OPT_GROW   2

/*
 * Malus/Bonus
 */

# define NORM		0
# define BLOCK		1
# define FAST		2
# define SLOWER		3

/*
 * The Snake is a list.
 */

typedef struct	s_snake		t_snake;

struct			s_snake
{
  char			type;
  int			x;
  int			y;
  struct s_snake	*next;
};

/*
 * Porotype.
 */

void		artificialPlay(int x_eat, int y_eat,
			       int yMax, int xMax,
			       int *goKey, int *lastKey,
			       t_snake *snake);
int		inBody(int y, int x, t_snake *snake);
int		whichKey(int y, int x,
			 int goKey, int lastKey,
			 t_snake *snake,
			 int max_x, int max_y);
int		playGame(int key, int x, int y, int flag);
int		usleep(int usec);
int		getCmdLine(WINDOW *win, int flag);
void		creatBorder(int xmax, int ymax, int token);
t_snake		*initSnake(int y, int x);
void		growSnake(t_snake **snake);
int		nbrFigure(int nbr);

#endif /* SNAKE_H */
