#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#define P '^' // player
#define A '#' // astroids
#define E ' ' // empty space
#define X 40 // width
#define Y 20 // hight

#define N_A 4 // number of astroids


int px = X/2,py = Y/2;
int l = 0,r = 0,u = 0,d = 0;

int astroid_x[N_A],astroid_y[N_A];
int move_ast[N_A];

void print_world()
{
	for(int i = 0;i <= Y;i++)
	{
		for(int j = 0;j <= X;j++)
		{
			int is_astroid = 0;
			for(int k = 0;k < N_A;k++)
			{
				if(i == astroid_y[k] 
				&& j == astroid_x[k]) 
				{
					printw("%c",A);
					is_astroid = 1;
				}
			}
			if(i == Y/2 && j == X/2) printw("%c",P);
			else if(!is_astroid) printw("%c",E);
		}
		printw("\n");
	}
}


void init_astroid()
{
	for(int i = 0;i < N_A;i++)
	{
		astroid_x[i] = rand() % X;
		astroid_y[i] = rand() % Y;
	}
}

void astroid_direction()
{
	for(int i = 0;i < N_A;i++)
	{
		move_ast[i] = rand() % 8;
	}
}

void move_astroid()
{
	for(int i = 0;i < N_A;i++)
	{
		switch(move_ast[i])
		{
			case 0: // move up
				astroid_y[i]--;
				break;
			case 1: // move up-left
				astroid_y[i]--;
				astroid_x[i]--;
				break;
			case 2: // move up-right
				astroid_y[i]--;
				astroid_x[i]++;
				break;
			case 3: // move down
				astroid_y[i]++;
				break;
			case 4: // move down-left
				astroid_y[i]++;
				astroid_x[i]--;
				break;
			case 5: // move down-right
				astroid_y[i]++;
				astroid_x[i]++;
				break;
			case 6: // move right
				astroid_x[i]++;
				break;
			case 7: // move left
				astroid_x[i]--;
				break;
		}

		if(astroid_x[i] >= X) astroid_x[i] = 0;
		else if(astroid_x[i] < 0) astroid_x[i] = X - 1;
		if(astroid_y[i] >= Y) astroid_y[i] = 0;
		else if(astroid_y[i] < 0) astroid_y[i] = Y - 1;
	}
}

void lose_game()
{
	int lose = 0;
	for(int i = 0;i < N_A;i++)
	{
		if(astroid_x[i] == px && astroid_y[i] == py)
		{
			lose = 1;
			break;
		}
	}
	if(lose)
	{
		mvprintw(Y / 2, X / 2 - 5, "GAME OVER");
        	refresh();
        	usleep(2000000);  // Pause for 2 s
		init_astroid();
		print_world();
		astroid_direction();
		px = X/2;
		py = Y/2;
	}
}

void update_world()
{
	mvprintw(py,px, "%c",P);
	for(int i = 0;i < N_A;i++)
	{
		mvprintw(astroid_y[i],astroid_x[i], "%c",A);
	}
}
void player_move()
{
	if(l) px--;
	else if(r) px++;
	if(d) py++;
	else if(u) py--;

	// colision
	if(px >= X) px = 0;
	else if(px < 0) px = X - 1;
	if(py >= Y) py = 0;
	else if(py < 0) py = Y - 1;
}

void handle_input(int ch)
{
	if(ch != ERR)
	{
		switch(ch)
		{
			case 'z':
			case KEY_UP:
				u = 1;
				break;
			case 's':
			case KEY_DOWN:
				d = 1;
				break;
			case 'q':
			case KEY_LEFT:
				l = 1;
				break;
			case 'd':
			case KEY_RIGHT:
				r = 1;
				break;
			default:
				break;
		}
	}
	else u = d = l = r = 0;

}

int main()
{
	initscr();          // Initialize the ncurses mode
	timeout(0);         // Non-blocking input
	noecho();           // Disable key echoing
    	curs_set(0);        // Hide the cursor
	int ch;

	init_astroid();
	print_world();
	astroid_direction();

	while((ch = getch()) != 'p')
	{
		clear();
		// input and player move
		handle_input(ch);
		player_move();

		// astroid
		move_astroid();
		
		// world update
		update_world();
		lose_game();

		refresh();
		usleep(100000);
	}
	clear();
	endwin();
	return 0;
}
