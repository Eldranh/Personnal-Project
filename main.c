/*
 * CMD Line
 *
 * Made by MAMODE Karim
 * The 25th of August 2011 at 15h00
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

int		playGame(int key, int x, int y);
int		usleep(int usec);

typedef struct	s_snake		t_snake;

struct		s_snake
{
  int			x;
  int			y;
  struct s_snake	*next;
};

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
	  else
	    y = max_y - 2;
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, '^');
	}
      else
	{
	  goKey = KEY_DOWN;
	  if (y < max_y - 2)
	    y++;
	  else
	    y = 1;
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, 'v');
	}
      break;
    case KEY_DOWN:
      if (lastKey != KEY_UP)
	{
	  if (y < max_y - 2)
	    y++;
	  else
	    y = 1;
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, 'v');
	}
      else
	{
	  goKey = KEY_UP;
	  if (y > 1)
	    y--;
	  else
	    y = max_y - 2;
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, '^');
	}
      break;
    case KEY_LEFT:
      if (lastKey != KEY_RIGHT)
	{
	  if (x > 2)
	    x -= 2;
	  else
	    x = max_x - 3 - ((max_x + 1) % 2);
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, '<');
	}
      else
	{
	  goKey = KEY_RIGHT;
	  if (x < max_x - 4)
	    x += 2;
	  else
	    x = 2;
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, '>');
	}
      break;
    case KEY_RIGHT:
      if (lastKey != KEY_LEFT)
	{
	  if (x < max_x - 4)
	    x += 2;
	  else
	    x = 2;
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, '>');
	}
      else
	{
	  goKey = KEY_LEFT;
	  if (x > 2)
	    x -= 2;
	  else
	    x = max_x - 3 - ((max_x + 1) % 2);
	  snake->x = x;
	  snake->y = y;
	  mvaddch(y, x, '<');
	}
      break;
    default:
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
  (*snake) = (*snake)->next;
}

int		playGame(int key, int max_x, int max_y)
{
  int		x_eat = 0;
  int		y_eat = 0;
  int		token = 0;
  int		goKey = KEY_UP;
  int		lastKey = 0;
  int		x = max_x / 2 + ((max_x + 1) % 2);
  int		y = max_y / 2;
  int		handicap = 0;
  int		size = 0;
  int		tm = 0;
  int		score = 0;
  int		laps = 0;
  t_snake	*snake;
  double	timeLeast = 30.;

  attron(COLOR_PAIR(1));
  mvprintw(max_y - 1, 0, "Good Luck !");
  attroff(COLOR_PAIR(1));
  curs_set(0);
  snake = malloc(sizeof(t_snake*));
  if (snake == NULL)
    exit(EXIT_FAILURE);
  snake->x = x;
  snake->y = y;
  snake->next = malloc(sizeof(t_snake*));
  if (snake->next == NULL)
    exit(EXIT_FAILURE);
  snake->next->next = snake; 
  snake->next->x = x - 2;
  snake->next->y = y - 1;
  while (key != 'q')
    {
      x = snake->x;
      y = snake->y;
      if (token == 1)
	{
	  snake = snake->next;
	  mvaddch(snake->y, snake->x, ' ');
	}
      while (token == 0)
	{
	  x_eat = (rand() % (max_x - 5)) + 2;
	  if (x_eat % 2)
	    x_eat++;
	  y_eat = (rand() % (max_y - 3)) + 1;
	  mvaddch(y_eat, x_eat, '*');
	  if (!inBody(y_eat, x_eat, snake))
	    {
	      token = 1;
	      tm = time(0);
	      growSnake(&snake);
	    }
	}
      usleep(150000 - handicap);
      timeLeast -= (150000.0 - handicap) / 1000000.0;
      refresh();

      if ((key = getch()) != ERR)
	{
	  lastKey = goKey;
	  goKey = key;
	}

      goKey = whichKey(y, x, goKey, lastKey, snake, max_x, max_y);

      if (inBody(snake->y, snake->x, snake))
	return (score);
      if ((x == x_eat && y == y_eat)
	  || time(0) - tm >= (30 - (handicap / 5000)))
	{
	  if ((laps = time(0) - tm) >= (30 - (handicap / 5000)))
	    {
	      mvaddch(y_eat, x_eat, ' ');
	      score -= (100 - 3 * size);
	    }
	  else
	    {
	      if (handicap / 5000 <= 27)
		handicap += 5000;
	      size++;
	      score += (10 - laps) * size;
	      move(max_y - 1, max_x - 1 - nbrFigure(size));
	    }
	  token = 0;
	  timeLeast = (30 - (handicap / 5000));
	}
      attron(COLOR_PAIR(1));
      mvprintw(max_y - 1, 0, " -- Score : %d -- Time before next : %.0f -- Size : %d -- Speed : %d -- ", score, timeLeast, size, handicap / 1000);
      if (timeLeast < 10.)
	addch('*');
      attroff(COLOR_PAIR(1));
    }
  return (0);
}

void		creatBorder(int xmax, int ymax)
{
  int		x = 0;
  int		y = 0;

  clear();
  while (x < xmax)
    {
      mvaddch(0, x, '^');
      x++;
    }
  x = 0;
  while (x < xmax)
    {
      mvaddch(ymax - 1, x, 'v');
      x++;
    }
  while (y < ymax)
    {
      mvaddch(y, 0, '<');
      y++;
    }
  y = 0;
  while (y < ymax)
    {
      mvaddch(y, xmax - 1, '>');
      y++;
    }
  y = 0;
  while (y < ymax)
    {
      mvaddch(y, 1, '<');
      y++;
    }
  y = 0;
  while (y < ymax)
    {
      mvaddch(y, xmax - 2, '>');
      y++;
    }
  y = 0;
}

int		getCmdLine(WINDOW *win)
{
  int		max_x;
  int		max_y;
  int		key = 0;
  int		score;

  srand(getpid() * time(0));
  getmaxyx(win, max_y, max_x);
  attron(COLOR_PAIR(1));
  creatBorder(max_x, max_y);
  attroff(COLOR_PAIR(1));
  score = playGame(key, max_x, max_y);
  
  mvaddstr(max_y / 2, (max_x - strlen("YOU FAILED !!!!")) / 2, "YOU FAILED !!!!");
  mvaddstr(max_y / 2 + 1, (max_x - strlen("Press y to start a new game or q to quit...")) / 2, "Press y to start a new game or q to quit...");
  mvaddstr(max_y / 2 + 3, (max_x - strlen("Your score is...")) / 2, "Your score is...");
  mvprintw(max_y / 2 + 4, (max_x - nbrFigure(score)) / 2, "%d", score);
  while (key != 'q' && key != 'y'
	 && key != 'Q' && key != 'Y')
    key = getch();
  if (key == 'y' || key == 'Y')
    getCmdLine(win);
  return (0);
}

int		main(int argc, char **argv)
{
  WINDOW	*win;

  (void)argc;
  (void)argv;
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

  init_color(COLOR_BLACK, 0, 0, 0);
  init_color(COLOR_WHITE, 255, 255, 255);
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
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
  getCmdLine(win);
  refresh();
  endwin();
  printf("Thanks for using my test.\n");
  return (EXIT_SUCCESS);
}
