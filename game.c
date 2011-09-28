/*
 * File Name :
 * Purpose :
 * Creation Date :
 * Last Modified : jeu. 22 sept. 2011 15:11:01 CEST
 * Created By :
 */

#include	<curses.h>
#include	<term.h>
#include	<string.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<math.h>
#include	<time.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	"snake.h"

int		playGame(int key, int max_x, int max_y, int flag)
{
  int		x_eat = 0;
  int		y_eat = 0;
  int		token = 0;
  int		goKey = KEY_UP;
  int		lastKey = 0;
  int		x = max_x / 2 + ((max_x + 1) % 2);
  int		y = max_y / 2;
  int		handicap = 0;
  int		size = 2;
  int		tm = 0;
  int		score = 0;
  int		laps = 0;
  t_snake	*snake;
  double	timeLeast = 30.;
  int		random;
  char		type = NORM;

  attron(COLOR_PAIR(1));
  mvprintw(max_y - 1, 0, "Good Luck !");
  attroff(COLOR_PAIR(1));
  curs_set(0);
  snake = initSnake(y, x);
  while (key != 'q')
    {
      x = snake->x;
      y = snake->y;
      if (token == 1)
	{
	  if ((flag & OPT_IA) == OPT_IA)
	    artificialPlay(x_eat, y_eat, max_y, max_x, &goKey, &lastKey, snake);
	  snake->next->type = snake->type;
	  snake = snake->next;
	  if (!((flag & OPT_GROW) == OPT_GROW))
	    mvaddch(snake->y, snake->x, ' ');
	}
      while (token == 0)
	{
	  x_eat = (rand() % (max_x - 5)) + 2;
	  if (x_eat % 2)
	    x_eat++;
	  y_eat = (rand() % (max_y - 3)) + 1;
	  if ((flag & OPT_IA) == OPT_IA)
	    artificialPlay(x_eat, y_eat, max_y, max_x, &goKey, &lastKey, snake);
	  random = rand() % 15;
	  if (0 <= random && random < 3)
	    {
	      type = BLOCK;
	      attron(COLOR_PAIR(3));
	      mvaddch(y_eat, x_eat, 'X');
	    }
	  else if (3 <= random && random < 6)
	    {
	      type = FAST;
	      attron(COLOR_PAIR(2));
	      mvaddch(y_eat, x_eat, '+');
	    }
	  else if (random == 7)
	    {
	      type = SLOWER;
	      attron(COLOR_PAIR(4));
	      mvaddch(y_eat, x_eat, 'o');
	    }
	  else
	    {
	      type = NORM;
	      mvaddch(y_eat, x_eat, '*');
	    }
	  if (type == BLOCK)
	    attroff(COLOR_PAIR(3));
	  else if (type == FAST)
	    attroff(COLOR_PAIR(2));
	  else if (type == SLOWER)
	    attroff(COLOR_PAIR(4));
	  if (!inBody(y_eat, x_eat, snake))
	    {
	      token = 1;
	      tm = time(0);
	      if (!((flag & OPT_GROW) == OPT_GROW))
		growSnake(&snake);
	    }
	}
      if ((flag & OPT_GROW) == OPT_GROW)
	growSnake(&snake);
      if (snake->type == FAST)
	{
	  usleep(5000);
	  timeLeast -= (5000.0) / 1000000.0;
	}
      else
	{
	  usleep(150000 - handicap);
	  timeLeast -= (150000.0 - handicap) / 1000000.0;
	}
      refresh();

      if (!((flag & OPT_IA) == OPT_IA))
	{
	  if ((key = getch()) != ERR)
	    {
	      lastKey = goKey;
	      goKey = key;
	    }
	}

      goKey = whichKey(y, x, goKey, lastKey, snake, max_x, max_y);
      if (goKey == -1)
	return (score);

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
	      if (handicap / 5000 <= 28)
		handicap += 5000;
	      size++;
	      score += (10 - laps) * size;
	      move(max_y - 1, max_x - 1 - nbrFigure(size));
	      if (type == BLOCK)
		creatBorder(max_x, max_y, 0);
	      else
		creatBorder(max_x, max_y, 1);
	      if (type == SLOWER)
		{
		  snake->type = NORM;
		  handicap -= 10000;
		}
	      else
		snake->type = type;
	    }
	  token = 0;
	  timeLeast = (30 - (handicap / 5000));
	}
      if (snake->type != BLOCK)
	attron(COLOR_PAIR(1));
      mvprintw(max_y - 1, 0, " -- Score : %d -- Time before next : %.0f -- Size : %d -- Speed : %d -- ", score, timeLeast, size, handicap / 1000);
      if (timeLeast < 10.)
	addch('*');
      if (snake->type != BLOCK)
	attroff(COLOR_PAIR(1));
    }
  return (0);
}

int		getCmdLine(WINDOW *win, int flag)
{
  int		max_x;
  int		max_y;
  int		key = 0;
  int		score;
  char		*str = NULL;

  srand(getpid() * time(0));
  getmaxyx(win, max_y, max_x);
  creatBorder(max_x, max_y, 2);
  score = playGame(key, max_x, max_y, flag);

  str = "YOU FAILED !!!!";
  mvaddstr(max_y / 2, (max_x - strlen(str)) / 2, str);
  str = "Press y to start a new game or q to quit...";
  mvaddstr(max_y / 2 + 1, (max_x - strlen(str)) / 2, str);
  str = "Your score is...";
  mvaddstr(max_y / 2 + 3, (max_x - strlen(str)) / 2, str);
  mvprintw(max_y / 2 + 4, (max_x - nbrFigure(score)) / 2, "%d", score);
  while (key != 'q' && key != 'y'
	 && key != 'Q' && key != 'Y')
    key = getch();
  if (key == 'y' || key == 'Y')
    getCmdLine(win, flag);
  return (0);
}

