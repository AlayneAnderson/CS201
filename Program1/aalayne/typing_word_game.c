/*Alayne Anderson CS201 Program 1
This program is a typind speed game. The user is prompted 
to type a randon word out of nine words*/

#include <stdio.h>
#include <stdlib.h> /*required to use rand() and srand()*/
#include <sys/time.h>
#include <time.h>
#include <string.h>

/*function declarations*/
void play(void); /*manages the game */
int rand_num(int total); /*Generates a random #*/
double time_diff(struct timeval time_start, struct timeval time_end); /*Gets the time it took to play the game*/
int again(); /*asks the user if they want to play again*/

int main()
{

	struct timeval tv;
	do
	{
		gettimeofday(&tv, NULL); /* to get the time in seconds to use as the seed for srand*/
		srand(tv.tv_usec); /*For randomization*/
		
		play();
	}while(again()==1); /*Play the game until the user wants to quit*/
	
	return 0;
}

/*This function manages the game*/
void play(void)
{
	struct timeval before, after;
	int total=8;
	int index=0;
	int i=0;
	int max=0;
	char str[10];
	
	/*Create an array of strings */
	char words[9][6]={"The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"};
	/*Create an array of numbers */
	int num[9]={0,1,2,3,4,5,6,7,8};

	printf("Welcome to the Typing speed game! \n Type these words as fast as you can!");
	printf("\n Ready? \n Press ENTER to start,");
	getchar();

	gettimeofday(&before, NULL); /*Gets the time at the start of the game*/
	
	while(total > 0) /*randomize the array of integers*/
	{
		index= rand_num(total);

		max=num[total];
		num[total]=num[index];
		num[index]=max;
		--total;
	}

	for(i=0; i<9; ++i)/* Print the word that correpsponds to the number in the array*/
	{
		do
		{
			printf("\nWord #%d is %s: ", i+1, words[num[i]]); /*Print the word*/
			scanf("%9s",str); /*input the word from the user*/
			if(strcmp(str, words[num[i]]) !=0)
				printf("Incorrect. Try again.\n");
		}while(strcmp(str, words[num[i]]) !=0);
	}
	
	gettimeofday(&after, NULL);

	/*Calculate the difference between the start & end time */
	printf("\n\nCorrect! Your time is: %.0f sec, %.0f use", (time_diff(before, after)*1.0e-6), time_diff(before,after));			
	
}

/*This function takes in a number, total & returns
a random number between 0 and total */
int rand_num(int total)
{
	int num; 
	num=rand()%total; 
	return num;
}

/*Takes 2 times in microseconds as arguments and returns the difference in microseconds */
double time_diff(struct timeval time_start, struct timeval time_end)
{
	double time_start_ms, time_end_ms, time_diff;

	time_start_ms=(double)time_start.tv_sec*1000000+(double)time_start.tv_usec;
	time_end_ms= (double)time_end.tv_sec*1000000+(double)time_end.tv_usec;

	time_diff=(double)time_end_ms- (double)time_start_ms;
	
	return time_diff;
}

/*Prompts the user to play again. Returns true for 1 and false for 0*/
int again()
{
	char reply;
	printf("Would you like to play again (y/n)?");
	scanf("%c",&reply);
	if(reply =='y' || reply == 'Y')
		return 1;
	else
		return 0;
}
