/*
 * Snake Game
 *
 * Made by MAMODE Karim
 * Created he 25th of August 2011 at 15h00
 * Last Modification the 8th of September 2011 at 11h10
 *
 * Train to bring the most of us.
 */

#include	<string.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<curses.h>
#include	<term.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<math.h>
#include	<time.h>
#include	"snake.h"

int		nbrFigure(int nbr)
{
  int		size = 1;

  while (nbr / 10)
    {
      nbr /= 10;
      size++;
    }
  return (size);
}

int		inBody(int y, int x, t_snake *snake)
{
  t_snake	*compare;

  compare = snake;
  snake = snake->next;
  while (snake != compare)
    {
      if (snake->x == x && snake->y == y)
	return (1);
      snake = snake->next;
    }
  return (0);
}

int		whichKey(int y, int x,
			 int goKey, int lastKey,
			 t_snake *snake,
			 int max_x, int max_y)
{
  switch (goKey)
    {
    case KEY_UP:
     if (lastKey != KEY_DOWN)
       {
	 if (y > 1)
	   y--;
	 else if (snake->type == BLOCK)
	   return (-1);
	 else
	   y = max_y - 2;
	 snake->x = x;
	 snake->y = y;
	 mvaddch(y, x, '^');
       }
     else
       {
	 goKey = KEY_DOWN;
	 whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
       }
     break;
    case KEY_DOWN:
     if (lastKey != KEY_UP)
       {
	 if (y < max_y - 2)
	   y++;
	 else if (snake->type == BLOCK)
	   return (-1);
	 else
	   y = 1;
	 snake->x = x;
	 snake->y = y;
	 mvaddch(y, x, 'v');
       }
     else
       {
	 goKey = KEY_UP;
	 whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
       }
     break;
    case KEY_LEFT:
     if (lastKey != KEY_RIGHT)
       {
	 if (x > 2)
	   x -= 2;
	 else if (snake->type == BLOCK)
	   return (-1);
	 else
	   x = max_x - 3 - ((max_x + 1) % 2);
	 snake->x = x;
	 snake->y = y;
	 mvaddch(y, x, '<');
       }
     else
       {
	 goKey = KEY_RIGHT;
	 whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
       }
     break;
    case KEY_RIGHT:
     if (lastKey != KEY_LEFT)
       {
	 if (x < max_x - 4)
	   x += 2;
	 else if (snake->type == BLOCK)
	   return (-1);
	 else
	   x = 2;
	 snake->x = x;
	 snake->y = y;
	 mvaddch(y, x, '>');
       }
     else
       {
	 goKey = KEY_LEFT;
	 whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
       }
     break;
    case 'p':
     while (getch() != 'p');
     goKey = lastKey;
     whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
     break;
    default:
     goKey = lastKey;
     whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
     break;
    }
  return (goKey);
}

void		growSnake(t_snake **snake)
{
  t_snake	*save;

  save = (*snake)->next;
  (*snake)->next = malloc(sizeof(**snake));
  if ((*snake)->next == NULL)
    exit(EXIT_FAILURE);
  (*snake)->next->next = save;
  (*snake)->next->type = (*snake)->type;
  (*snake) = (*snake)->next;
}

t_snake		*initSnake(int y, int x)
{
  t_snake	*snake;

  snake = malloc(sizeof(*snake));
  if (snake == NULL)
    exit(EXIT_FAILURE);
  snake->x = x;
  snake->y = y;
  snake->type = NORM;
  snake->next = malloc(sizeof(*snake));
  if (snake->next == NULL)
    exit(EXIT_FAILURE);
  snake->next->next = snake; 
  snake->next->x = x - 2;
  snake->next->y = y - 1;
  snake->next->type = snake->type;
  return (snake);
}

int		main(int argc, char **argv)
{
  WINDOW	*win;
  int		flag = 0;
  int		i = 1;

  while (argc > i)
    {
      if (argv[i][0] == '-')
	argv[i]++;
      if (argv[i]
	  && ((!strcmp(argv[i], "-full-grow") && i++)
	      || (argv[i][0] == 'g' && argv[i]++)))
	flag += OPT_GROW;
      if (argv[i]
	  && ((!strcmp(argv[i], "-artificial-intelligence") && i++)
	      || (argv[i][0] == 'i' && argv[i]++)))
	flag += OPT_IA;
      if (argv[i] != NULL && !argv[i][0])
	i++;
    }
  if ((win = initscr()) == NULL)
    {
      fprintf(stderr, "Can't creat a new window.\nExiting Soft.\n");
      return (EXIT_FAILURE);
    }
  if (!has_colors() || start_color() == ERR)
    {
      endwin();
      fprintf(stderr, "Can't enable color.\nExiting Soft.\n");
      return (EXIT_FAILURE);
    }

  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);

  if (noecho() == ERR)
    {
      endwin();
      fprintf(stderr, "Can't disable echo.\nExiting Soft.\n");
      return (EXIT_FAILURE);
    }
  if (keypad(win, 1) == ERR)
    {
      endwin();
      fprintf(stderr, "Can't simplify keystroke.\nExiting Soft.\n");
      return (EXIT_FAILURE);
    }
  nodelay(win, 1);
  getCmdLine(win, flag);
  refresh();
  endwin();
  printf("Thanks for using my test.\n");
  return (EXIT_SUCCESS);
}
