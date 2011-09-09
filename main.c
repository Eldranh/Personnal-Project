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

int		playGame(int key, int x, int y, int flag);
int		usleep(int usec);

typedef struct	s_snake		t_snake;

# define OPT_IA     1
# define OPT_GROW   2

# define NORM		0
# define BLOCK		1
# define FAST		2
# define SLOWER		3

struct			s_snake
{
    char			type;
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

void		artificialPlay(int x_eat, int y_eat,
        int yMax, int xMax,
        int *goKey, int *lastKey,
        t_snake *snake)
{
    int		token = 1;
    int		dist = 0;
    int		coord_incr = 0;

    if (snake->type == BLOCK)
    {
        if ((snake->x - x_eat) != 0)
        {
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

    srand(getpid() * time(0));
    getmaxyx(win, max_y, max_x);
    creatBorder(max_x, max_y, 2);
    score = playGame(key, max_x, max_y, flag);

    mvaddstr(max_y / 2, (max_x - strlen("YOU FAILED !!!!")) / 2, "YOU FAILED !!!!");
    mvaddstr(max_y / 2 + 1, (max_x - strlen("Press y to start a new game or q to quit...")) / 2, "Press y to start a new game or q to quit...");
    mvaddstr(max_y / 2 + 3, (max_x - strlen("Your score is...")) / 2, "Your score is...");
    mvprintw(max_y / 2 + 4, (max_x - nbrFigure(score)) / 2, "%d", score);
    while (key != 'q' && key != 'y'
            && key != 'Q' && key != 'Y')
        key = getch();
    if (key == 'y' || key == 'Y')
        getCmdLine(win, flag);
    return (0);
}

int		    main(int argc, char **argv)
{
    WINDOW	*win;
    int     flag = 0;
    int     i = 0;

    while (argc > i)
    {
        if (!strcmp(argv[i], "--full-grow") || !strcmp(argv[i], "-g"))
            flag += OPT_GROW;
        if (!strcmp(argv[i], "--artificial-intelligence") || !strcmp(argv[i], "-i"))
            flag += OPT_IA;
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
