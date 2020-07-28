/*
Alayne Anderson
CS201
Program 1

Solves a maze, passed as an argument via right-hand wall solving algorithm*/




#include <stdlib.h>
#include <stdio.h>


/* Variables */
FILE *inFile = NULL;
int data_table[3][2];
char **maze = NULL;
int start[2];
int finish[2];
int position[2];
char orientation = 'S';
char found_home = 'N';


/* Prototypes */
void populate_table(FILE *);
void populate_maze(FILE *);
int check_left(char , int , int);
int check_right(char, int , int);
int check_forward(char, int , int);
int check_behind(char, int , int);
void phone_home(void);
void solve_maze(void);
void display_maze(void);
void reveal_path(void);
void deallocate_maze(void);


int main(int argc, char *argv[])
{

	inFile = fopen(argv[1], "r");
	if(inFile != NULL)
	{
		int i;
	
		/* If the file was opened, read in the first
		three lines and populate the data_table */
		populate_table(inFile);

		/* Allocate memory for the maze rows */
		maze = (char **)malloc(sizeof(char*) * data_table[0][1]);

		/* Allocate memory for the columns of the maze, this is
			done for each row in the maze */
		for(i=0; i < data_table[0][1]; ++i)
			maze[i] = malloc(sizeof(char) * (data_table[0][0] + 1));


		/* printing the array to ensure correct data was read in */
		fputs("\n\n.....Maze Data.....\n", stdout);
		printf("Dimensions: %d X %d\n", data_table[0][0], data_table[0][1]);
		printf("Entrance: %d , %d\n", start[1], start[0]);
		printf("Exit: %d , %d\n", finish[1], finish[0]);

		/* Once memory has been allocated for the maze
			read in data from file to build the maze */
		populate_maze(inFile);

		/* Display the maze that was read in from file */
		printf("\n\n.....Maze....\n");
		display_maze();

		/* Solve the maze, then display the solution */
		solve_maze();
		printf("\n\n....Maze Solved....\n");
		reveal_path(); /* Replaces 'A's with spaces, ' ' */
		display_maze();
		fputs("\n\n\nGoodbye!\n", stdout);


		deallocate_maze();
	}


	/* The file could not be opened */
	else
		fputs("The File Could Not Be Opened....See Ya!", stdout);
		
	fclose(inFile);
	return 0;
}






/* Function Bodies */



/* Reads in data from an external text file and uses
	the first three lines of the file to populate
	a data_table that holds the dimensions, starting
	and ending tiles for the maze. */
void populate_table(FILE *inF)
{
	char *ptr = NULL;
	char *data_line = (char*) malloc(sizeof(char) * 30);
	long int x,y;
	int i;

	/* Read in the first three lines of the file and use
		that data to populate the data_table */
	for(i=0; i <= 2; ++i)
	{
		data_line = fgets(data_line, 30, inF);
		x = strtol(data_line, &ptr, 10);
		++ptr;
		y = strtol(ptr, NULL, 10);

		data_table[i][0] = x;
		data_table[i][1] = y;
	}


	/* Set values for the start and finish
	arrays now that the data_table has been populate */	
	start[0] = data_table[1][1];
	start[1] = data_table[1][0];
	finish[0] = data_table[2][1];
	finish[1] = data_table[2][0];

	position[0] = start[0];
	position[1] = start[1];

	/* Now that the data_table values are populated,
		the memory allocated for the data_lines
		is no longer needed. */
	free(data_line);
}


/* Reads in maze data from the text file passed as
	an argument. This function assumes that the
	file has already been successfully opened. */
void populate_maze(FILE *inF)
{
	int i;

	/* skip the first three lines of the file */
	for(i=0; i <=2; ++i)
		fgets(maze[0], 0, inF);	


	/* For the height of the maze, read in data for	each row */
	for(i=0; i < data_table[0][1]; ++i)
		fgets(maze[i], data_table[0][0] + 2, inF);
}


/* Takes as arguments the orientation and current position. Based on the 
	orientation provided, if the tile to the left of the current position is
	not a wall, the function adjusts the orientation as needed, and
	moves the current position to the left of the current position. If
	the tile to the left of the current position is a wall, then no change to
	the orientation or position is made */
int check_left(char dir, int x, int y)
{
	if(dir == 'S')
	{
		if(maze[x][y+1] == 'X')
			return -1;
		
		if(maze[x][y+1] == '0' || maze[x][y+1] == 'A')
		{
			orientation = 'E';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y+1;
			return 1;
		}

	}
	
	else if(dir == 'N')
	{
		if(maze[x][y-1] == 'X')
			return -1;

		if(maze[x][y-1] == '0' || maze[x][y-1] == 'A')
		{
			orientation = 'W';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y-1;
			return 1;
		}
	}

	else if(dir == 'E')
	{
		if(maze[x-1][y] == 'X')
			return -1;

		if(maze[x-1][y] == '0' || maze[x-1][y] == 'A')
		{
			orientation = 'N';
			maze[x][y] = 'A';
			position[0] = x-1;
			position[1] = y;
			return 1;
		}
	}

	/* Direction is West */
	else
	{
		if(maze[x+1][y] == 'X')
			return -1;

		if(maze[x+1][y] == '0' || maze[x+1][y] == 'A')
		{
			orientation = 'S';
			maze[x][y] = 'A';
			position[0] = x +1;
			position[1] = y;
			return 1;
		}

	}	

	return 0;
}




/* Takes as arguments the orientation and current position. Based on the 
	orientation provided, if the tile to the right of the current position is
	not a wall, the function adjusts the orientation as needed, and
	moves the current position to the right of the current position. If
	the tile to the right of the current position is a wall, then no change to
	the orientation or position is made */
int check_right(char dir, int x, int y)
{
	if(dir == 'S')
	{
		if(maze[x][y-1] == 'X')
			return -1;
		
		if(maze[x][y-1] == '0' || maze[x][y-1] == 'A')
		{
			orientation = 'W';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y-1;
			return 1;
		}
	}
	
	else if(dir == 'N')
	{
		if(maze[x][y+1] == 'X')
			return -1;	

		if(maze[x][y+1] == '0' || maze[x][y+1] == 'A')
		{
			orientation = 'E';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y+1;
			return 1;	
		}
	}

	else if(dir == 'E')
	{
		if(maze[x+1][y] == 'X')
			return -1;	

		if(maze[x+1][y] == '0' || maze[x+1][y] == 'A')
		{
			orientation = 'S';
			maze[x][y] = 'A';
			position[0] = x+1;
			position[1] = y;
			return 1;
		}	
	}

	/* Direction is West */
	else
	{
		if(maze[x-1][y] == 'X')
			return -1;	

		if(maze[x-1][y] == '0' || maze[x-1][y] == 'A')
		{
			orientation = 'N';
			maze[x][y] = 'A';
			position[0] = x-1;
			position[1] = y;
			return 1;
		}
	}	


	return 0;
}



/* Takes as arguments the orientation and current position. Based on the 
	orientation provided, if the tile infront of the current position is
	not a wall, the function adjusts the orientation as needed, and
	moves the current position to one infront of the current position. If
	the tile infront of the current position is a wall, then no change to
	the orientation or position is made */
int check_forward(char dir, int x, int y)
{
	if(dir == 'S')
	{
		if(maze[x+1][y] == 'X')
			return -1;	
		
		else if(maze[x+1][y] == '0' || maze[x+1][y] == 'A')
		{
			orientation = 'S';
			maze[x][y] = 'A';
			position[0] = x+1;
			position[1] = y;
			return 1;	
		}
	}
	
	else if(dir == 'N')
	{
		if(maze[x-1][y] == 'X')
			return -1;	

		else if(maze[x-1][y] == '0' || maze[x-1][y] == 'A')
		{
			orientation = 'N';
			maze[x][y] = 'A';
			position[0] = x-1;
			position[1] = y;
			return 1;
		}	
	}

	else if(dir == 'E')
	{
		if(maze[x][y+1] == 'X')
			return -1;	

		if(maze[x][y+1] == '0' || maze[x][y+1] == 'A')
		{
			orientation = 'E';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y+1;
			return 1;
		}	
	}

	/* Direction is West */
	else
	{
		if(maze[x][y-1] == 'X')
			return -1;	

		if(maze[x][y-1] == '0' || maze[x][y-1] == 'A')
		{
			orientation = 'W';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y-1;
			return 1;	
		}
	}	


	return 0;
}



/* Takes as arguments the orientation and current position. Based on the 
	orientation provided, if the tile behind the current position is
	not a wall, the function adjusts the orientation as needed, and
	moves the current position to one behind the current position. If
	the tile behind the current position is a wall, then no change to
	orientation or position is made. */
int check_behind(char dir, int x, int y)
{
	if(dir == 'S')
	{
		if(maze[x-1][y] == 'X')
			return -1;	
		
		if(maze[x-1][y] == '0' || maze[x-1][y] == 'A')
		{
			orientation = 'N';
			maze[x][y] = 'A';
			position[0] = x-1;
			position[1] = y;
			return 1;
		}	
	}
	
	else if(dir == 'N')
	{
		if(maze[x+1][y] == 'X')
			return -1;	

		if(maze[x+1][y] == '0' || maze[x+1][y] == 'A')
		{
			orientation = 'S';
			maze[x][y] = 'A';
			position[0] = x+1;
			position[1] = y;
			return 1;
		}	
	}

	else if(dir == 'E')
	{
		if(maze[x][y-1] == 'X')
			return -1;	

		if(maze[x][y-1] == '0' || maze[x][y-1] == 'A')
		{
			orientation = 'W';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y-1;
			return 1;
		}	
	}

	/* Direction is West */
	else
	{
		if(maze[x][y+1] == 'X')
			return -1;	

		if(maze[x][y+1] == '0' || maze[x][y+1] == 'A')
		{
			orientation = 'E';
			maze[x][y] = 'A';
			position[0] = x;
			position[1] = y+1;
			return 1;
		}	
	}	

	return 0;
}



/* Checks the current position against the exit of the maze.
	if the current position matches the exit, the found_home
	variable to changed to 'Y', to indicate that the end of
	the maze has been reached */
void phone_home(void)
{
	if(position[0] == finish[0] && position[1] == finish[1])
	{
		found_home = 'Y';
		maze[position[0]][position[1]] = 'A';
	}
}


/* As long as home (or finish, if you prefer) has not been
	found, the function will continue to navigate the maze.
	After each move, the function "phones home" to see if 
	its current position matches the exit position to the 
	maze. If it is determined that the current position
	matches the exit of the maze, the function stops */
void solve_maze(void)
{
	while(found_home != 'Y')
	{
		if(check_right(orientation, position[0], position[1]) > 0)
			phone_home();

		
		else if(check_forward(orientation, position[0], position[1]) > 0)
			phone_home();

		
		else if(check_left(orientation, position[0], position[1]) > 0)
			phone_home();
		
		else
		{
			if(check_behind(orientation, position[0], position[1]) > 0)
				phone_home();
		}
	}
}


/* Displays the maze */
void display_maze(void)
{
	int i;
	for(i=0; i < data_table[0][1]; ++i)
		fputs(maze[i], stdout);
}


/* After the maze has been solved, the function
	iterates over the maze and replaces all
	'A's with blank spaces to improve readability
	of the solution to the maze. */
void reveal_path(void)
{
	int i,j;

	for(i=0; i < data_table[0][1]; ++i)
	{
		for(j=0; j < data_table[0][0]; ++j)
		{
			if(maze[i][j] == 'A')
				maze[i][j] = ' ';
		}
	}
}


/* Deallocates dynamic memory that was used
	in the creation of the maze */
void deallocate_maze(void)
{
	int i;
	for(i=0; i < data_table[0][1]; ++i)
		free(maze[i]);
}



