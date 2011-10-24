/*
 * File Name :
 * Purpose :
 * Creation Date :
 * Last Modified : jeu. 22 sept. 2011 15:19:08 CEST
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

void		creatBorder(int xmax, int ymax, int token)
{
  int		x = 0;
  int		y = 0;

  if (token == 2)
    clear();
  if (token)
    attron(COLOR_PAIR(1));
  while (x < xmax)
    {
      if (!token)
	mvaddch(0, x, '-');
      else
	mvaddch(0, x, '^');
      x++;
    }
  x = 0;
  while (x < xmax)
    {
      if (!token)
	mvaddch(ymax - 1, x, '-');
      else
	mvaddch(ymax - 1, x, 'v');
      x++;
    }
  while (y < ymax)
    {
      if (!token)
	mvaddch(y, 0, '|');
      else
	mvaddch(y, 0, '<');
      y++;
    }
  y = 0;
  while (y < ymax)
    {
      if (!token)
	mvaddch(y, xmax - 1, '|');
      else
	mvaddch(y, xmax - 1, '>');
      y++;
    }
  y = 0;
  while (y < ymax)
    {
      if (!token)
	mvaddch(y, 1, '|');
      else
	mvaddch(y, 1, '<');
      y++;
    }
  y = 0;
  while (y < ymax)
    {
      if (!token)
	mvaddch(y, xmax - 2, '|');
      else
	mvaddch(y, xmax - 2, '>');
      y++;
    }
  if (token)
    attroff(COLOR_PAIR(1));
  y = 0;
}
