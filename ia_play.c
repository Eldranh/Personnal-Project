/*
 * File Name :
 * Purpose :
 * Creation Date :
 * Last Modified : jeu. 22 sept. 2011 15:09:13 CEST
 * Created By :
 */

#include	<curses.h>
#include	<term.h>
#include	"snake.h"

void		artificialPlay(int x_eat, int y_eat,
			       int yMax, int xMax,
			       int *goKey, int *lastKey,
			       t_snake *snake)
{
  int		token = 1;
  int		dist = 0;
  int		coord_incr = 0;

  if (snake->type == BLOCK) {
      if ((snake->x - x_eat) != 0) {
	  if (*goKey != KEY_RIGHT && (snake->x - x_eat) > 0)
	    {
	      *lastKey = *goKey;
	      *goKey = KEY_LEFT;
	      token = 0;
	    }
	  else if ((snake->x - x_eat) < 0 && *goKey != KEY_LEFT)
	    {
	      *lastKey = *goKey;
	      *goKey = KEY_RIGHT;
	      token = 0;
	    }
      }
      if (token)
	{
	  if (*goKey != KEY_UP && (snake->y - y_eat) < 0)
	    {
	      *lastKey = *goKey;
	      *goKey = KEY_DOWN;
	    }
	  else if (*goKey != KEY_DOWN && (snake->y - y_eat) > 0)
	    {
	      *lastKey = *goKey;
	      *goKey = KEY_UP;
	    }
	}
  }
  else
    {
      coord_incr = snake->y;
      while (coord_incr != y_eat)
	{
	  coord_incr++;
	  if (coord_incr > yMax - 1)
	    coord_incr = 1;
	  dist++;
	}
      if (dist && dist <= yMax / 2 && *goKey != KEY_UP)
	{
	  *lastKey = *goKey;
	  *goKey = KEY_DOWN;
	}
      else if (dist && *goKey != KEY_DOWN)
	{
	  *lastKey = *goKey;
	  *goKey = KEY_UP;
	}
      else
	{
	  dist = 0;
	}
      if (!dist)
	{
	  coord_incr = snake->x;
	  while (coord_incr != x_eat)
	    {
	      coord_incr += 2;
	      if (coord_incr > xMax - 2)
		coord_incr = 2;
	      dist += 2;
	    }
	  if (*goKey != KEY_RIGHT && dist && dist >= xMax / 2)
	    {
	      *lastKey = *goKey;
	      *goKey = KEY_LEFT;
	    }
	  else if (dist && *goKey != KEY_RIGHT)
	    {
	      *lastKey = *goKey;
	      *goKey = KEY_RIGHT;
	    }
	}
    }
}
