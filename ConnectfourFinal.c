/******************************************
*
* Official Name:  Alexander Cramer
*
* Nickname:  Alex
*
* E-mail:  alcramer@syr.edu
*
* Final Project:  Connect four
*
* Compiler:  Visual Studio 2017
*
* Date:  May 3, 2018
*
*******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define Board_Hor 40
#define Board_Vert 13
#define nameLength 20
//This is my struct that allows me to read in the highscores
typedef struct {			//	(STRUCT)
	char names[30];
	int wins;
	int loss;
	int draw;
}scoreType;

void menu();
void inputOutputfile();
int searchNames(scoreType highscores[], int total, char target[]);		//(STRUCT PARAM)
void new_oldnames(int index, scoreType highscores[]);
void intro(char player_1name[], char player_2name[], char sb[][Board_Hor]);
void vertline(int col, int startrow, int endrow, char sb[][Board_Hor]);
void horline(int row, int startcol, int endcol, char sb[][Board_Hor]);
void letters(int row, int startcol, int endcol, char symbol, char sb[][Board_Hor]);
void clearbuffer(char sb[][Board_Hor]);
void setBoard(char sb[][Board_Hor]);
void display(char sb[][Board_Hor]);
void switch_turn(char sb[][Board_Hor]);
void play_piece(int col, char symbol, char sb[][Board_Hor]);
void menu_choiceb(char player_1name[], char player_2name[], char sb[][Board_Hor]);
void checkwin_vert(char symbol, char sb[][Board_Hor]);
void checkwin_hor(char symbol, char sb[][Board_Hor]);
void checkwin_leftacross(char symbol, char sb[][Board_Hor]);
void checkwin_rightacross(char symbol, char sb[][Board_Hor]);
void checkWinner(char symbol, char sb[][Board_Hor]);
void play_computer(char player_1name[], char sb[][Board_Hor]);
void menu_choiceC(char player_1name[], char sb[][Board_Hor]);
void computer_turn(char *answer);
void swapscoretypes(scoreType x, scoreType y);
void select_sort(scoreType highscores[], int n);
int get_max_range(scoreType highscores[], int first, int last);
int main()
{
	srand(time(0));
	menu();
	return 0;
}
//The menu function is the first item to be called which gives you three choices about what 
//you want to do.
void menu()
{
	char sb[Board_Vert][Board_Hor];
	int row, col;
	char menuchoice, junk;
	char player_1name[20];
	char player_2name[20];

	printf("\t\tMENU\n");
	printf("To see highscores, press A\n");
	printf("Two player connect four, press B\n");
	printf("One player connect four, press C\n");
	printf("To quit, press Q\n");
	scanf(" %c", &menuchoice);
	switch (menuchoice)
	{
	case 'A':// The input file is used for printing out the highscores
	case 'a':	 inputOutputfile();
		break;
	case 'B': //Choice B allows you to play connect four
	case 'b':	menu_choiceb(player_1name, player_2name, sb);
		break;
	case 'C':
	case 'c': menu_choiceC(player_1name, sb);
		break;
	case 'Q': //quit
	case 'q': return (0);
		break;
	default: printf("Not an option\n");
		break;
	}
}
void menu_choiceb(char player_1name[], char player_2name[], char sb[][Board_Hor])
{
	char junk;
	//The first scanf into junk is used to get rid of the first enter sign which replaces a name
	scanf("%c", &junk);
	//intro to ask what each player name is
	intro(player_1name, player_2name, sb);

	clearbuffer(sb);
	setBoard(sb);
	display(sb);
	//This function call is how my game is mainly played
	//I use a switch statement to ask the user where they want their piece to go
	//This function also switched off between the players taking turns
	switch_turn(sb);
}
void intro(char player_1name[], char player_2name[], char sb[][Board_Hor])
{
	int index, total;
	char empty;
	FILE * MYIN = fopen("Connectfour_scores.txt", "r");
	scoreType highscores[10];
	fscanf(MYIN, "%d%c", &total, &empty);
	for (int i = 1; i <= total; i++)
	{
		fgets(highscores[i].names, nameLength, MYIN);
		empty = strlen(highscores[i].names);
		if (highscores[i].names[empty - 1] == '\n')
			highscores[i].names[empty - 1] = '\0';
		fscanf(MYIN, "%d%d%d%c", &highscores[i].wins, &highscores[i].loss, &highscores[i].draw, &empty);
	}
	fclose(MYIN);
	printf("Welcome to connect four!\n");
	printf("Player 1, please enter your name.\n");
	fgets(player_1name, nameLength, stdin);
	empty = strlen(player_1name);
	if (player_1name[empty - 1] == '\n')
		player_1name[empty - 1] = '\0';
	index = searchNames(highscores, total, player_1name);
	new_oldnames(index, highscores);
	printf("Player 2, please enter your name.\n");
	fgets(player_2name, nameLength, stdin);
	empty = strlen(player_2name);
	if (player_2name[empty - 1] == '\n')
		player_2name[empty - 1] = '\0';
	index = searchNames(highscores, total, player_2name);
	new_oldnames(index, highscores);

	printf("player 1 is %s who is red\n", player_1name);
	printf("player 2 is %s who is black\n", player_2name);
}
void inputOutputfile()								//		(I/O)
{
	int people, total;
	char goback, empty;
	FILE * MYIN;
	scoreType highscores[10];
	MYIN = fopen("Connectfour_scores.txt", "r");
	fscanf(MYIN, "%d%c", &total, &empty);
	//The first scanf statement shows that I read in the the first value which is the number of people 
	//on the highscore list. It then reads the new line into the empty character.
	printf("Names\t\t\t\tWins\t\tLoss\t\tDraw\n");
	for (int i = 1; i <= total; i++)
	{
		//This prints out the highscores in order from the input file.
		fgets(highscores[i].names, nameLength, MYIN);
		empty = strlen(highscores[i].names);
		if (highscores[i].names[empty - 1] == '\n')
			highscores[i].names[empty - 1] = '\0';
		fscanf(MYIN, "%d%d%d%c", &highscores[i].wins, &highscores[i].loss, &highscores[i].draw, &empty);
		printf("%s			%d		%d		%d\n", highscores[i].names, highscores[i].wins, highscores[i].loss, highscores[i].draw);
	}
	fclose(MYIN);
	//printf("before\n");
	select_sort(highscores, total);
	//printf("after\n");
	/*for (int i = 1; i <= total; i++)
	{
	printf("%s\t\t\t\t%d\t\t%d\t\t%d\n", highscores[i].names, highscores[i].wins, highscores[i].loss, highscores[i].draw);
	}*/
	FILE * MYOUT = fopen("ConnectfourOutput.txt", "w");
	for (int i = 1; i <= total; i++)
	{
		fprintf(MYOUT, "%s\t\t\t\t%d\t\t%d\t\t%d\n", highscores[i].names, highscores[i].wins, highscores[i].loss, highscores[i].draw);
	}
	fclose(MYOUT);
	menu();
}
void vertline(int col, int startrow, int endrow, char sb[][Board_Hor])
{
	//vertline is used to set the '|' parts of the board
	int i;
	for (i = startrow; i < endrow; i++)
		sb[i][col] = '|';
}
void horline(int row, int startcol, int endcol, char sb[][Board_Hor])
{
	//horline is used to set to '-' parts of the board
	int i;
	/* draw visible part of the line to screen buffer */
	for (i = startcol; i < endcol; i++)
		sb[row][i] = '-';
}
void letters(int row, int startcol, int endcol, char symbol,
	char sb[][Board_Hor])
{
	//letters is used to set to A-G parts of the board
	int i;
	for (i = startcol; i < endcol; i++)
		sb[row][i] = symbol;
}
void clearbuffer(char sb[][Board_Hor])
{
	int row, col;
	for (row = 0; row<Board_Vert; row++)
	{
		for (col = 0; col<Board_Hor; col++)
			sb[row][col] = ' ';
	}
}
void setBoard(char sb[][Board_Hor])
{
	//vertline is used to set to '|' parts of the board
	for (int i = 0; i <= 35; i += 5)
	{
		vertline(i, 0, 12, sb);
	}
	//horline is used to set the '-' parts of the board
	for (int i = 1; i <= 11; i += 2)
	{
		horline(i, 0, 36, sb);
	}
	//letters and rowlabel set the letters A-G on the bottom
	char rowLabel;
	rowLabel = 'A';
	for (int i = 3; i < 34; i += 5)
	{
		letters(12, i, i + 1, rowLabel, sb);
		rowLabel++;
	}
}
void display(char sb[][Board_Hor])
{
	// this section displays the board that I am using
	int row, col;
	for (row = 0; row < Board_Vert; row++)
	{
		for (col = 0; col < Board_Hor; col++)
			printf("%c", sb[row][col]);
		printf("\n");
	}
}
void play_piece(int col, char symbol, char sb[][Board_Hor])
{
	// If there is an item in the row and col spot you want, it will move
	// up 2 spaces to check the next row
	int row = 10;
	while (sb[row][col] != ' ')
	{
		//if there is a piece is the row below where you want to play your piece
		//it will go to the row above if it is an empty space.
		row -= 2;
	}
	sb[row][col] = symbol;
}
int searchNames(scoreType highscores[], int total, char target[])				//(SEARCH)
{
	//This function determines if the name you enter is already on the highscores list 
	//if it is, it will find where the location of your name is.
	for (int i = 0; i < total; i++)
	{
		if (strcmp(highscores[i].names, target) == 0)
			return i;
	}
	return -1;
}
void new_oldnames(int index, scoreType highscores[])
{
	//If the searchNames function comes back as -1, it says that this is your first time playing
	if (index == -1)
		printf("This is your first time playing\n");
	//If you are a returning player, it says your name along with your record. 
	else printf("Welcome back %s	Wins:%d		Loss:%d		Draw:%d\n", highscores[index].names, highscores[index].wins, highscores[index].loss, highscores[index].draw);
}
//The switch_turn function is where the game is mainly played. It continues to loop through this until a winner is declared. 
void switch_turn(char sb[][Board_Hor])
{
	int i;
	int gameover = 0;
	char answer, color;
	while (!gameover)
	{
		for (i = 1; i < 3; i++)
		{
			do {
				printf("Player %d, enter a letter A-G where you want your piece to go.\n", i);
				scanf(" %c", &answer);
				{ //switch colors after each turn
					if (i == 1)
						color = 'R';
					else
						color = 'B';
					switch (answer) //switch statement to allow the users to switch turns and play the game
					{
					case 'A':
					case 'a': play_piece(3, color, sb);
						break;
					case 'B':
					case 'b': play_piece(8, color, sb);
						break;
					case 'C':
					case 'c': play_piece(13, color, sb);
						break;
					case 'D':
					case 'd': play_piece(18, color, sb);
						break;
					case 'E':
					case 'e': play_piece(23, color, sb);
						break;
					case 'F':
					case 'f': play_piece(28, color, sb);
						break;
					case 'G':
					case 'g': play_piece(33, color, sb);
						break;
					default: printf("invalid choice\n");
					}//end switch
				}
			} while (!(answer == 'A' || answer == 'a' || answer == 'B' || answer == 'b' || answer == 'C' || answer == 'c' || answer == 'D' || answer == 'd' || answer == 'E' || answer == 'e' || answer == 'F' || answer == 'f' || answer == 'G' || answer == 'g'));
			display(sb);
			checkWinner(color, sb);

		}
	}
}
//This function tells you if there is a winner in the vertical direction
//If there is, it gives you the menu option
void checkwin_vert(char symbol, char sb[][Board_Hor])
{
	for (int col = 3; col <= 33; col += 5)
	{
		for (int row = 10; row >= 6; row -= 2)
		{
			if (sb[row][col] == symbol && sb[row - 2][col] == symbol)
				if (sb[row][col] == symbol && sb[row - 4][col] == symbol)
					if (sb[row][col] == symbol && sb[row - 6][col] == symbol)
						printf("%c won!\n\n", symbol);
			{
				if (sb[row][col] == symbol && sb[row - 2][col] == symbol)
					if (sb[row][col] == symbol && sb[row - 4][col] == symbol)
						if (sb[row][col] == symbol && sb[row - 6][col] == symbol)
							menu();
			}
		}
	}
}
//This function tells you if there is a winner in the horizontal direction
//If there is, it gives you the menu option
void checkwin_hor(char symbol, char sb[][Board_Hor])
{
	for (int row = 10; row >= 0; row -= 2)
	{
		for (int col = 3; col <= 18; col += 5)
		{
			if (sb[row][col] == symbol && sb[row][col + 5] == symbol)
				if (sb[row][col] == symbol && sb[row][col + 10] == symbol)
					if (sb[row][col] == symbol && sb[row][col + 15] == symbol)
						printf("%c won!\n\n", symbol);
			{
				if (sb[row][col] == symbol && sb[row][col + 5] == symbol)
					if (sb[row][col] == symbol && sb[row][col + 10] == symbol)
						if (sb[row][col] == symbol && sb[row][col + 15] == symbol)
							menu();
			}
		}
	}
}
//This function tells you if there is a winner in the left diagonal direction
//If there is, it gives you the menu option
void checkwin_leftacross(char symbol, char sb[][Board_Hor])
{
	for (int col = 33; col >= 18; col -= 5)
	{
		for (int row = 10; row >= 6; row -= 2)
		{
			if (sb[row][col] == symbol && sb[row - 2][col - 5] == symbol)
				if (sb[row][col] == symbol && sb[row - 4][col - 10] == symbol)
					if (sb[row][col] == symbol && sb[row - 6][col - 15] == symbol)
						printf("%c won!\n\n", symbol);
			{
				if (sb[row][col] == symbol && sb[row - 2][col - 5] == symbol)
					if (sb[row][col] == symbol && sb[row - 4][col - 10] == symbol)
						if (sb[row][col] == symbol && sb[row - 6][col - 15] == symbol)
							menu();
			}
		}
	}
}
//This function tells you if there is a winner in the right diagonal direction
//If there is, it gives you the menu option
void checkwin_rightacross(char symbol, char sb[][Board_Hor])
{
	for (int col = 3; col <= 18; col += 5)
	{
		for (int row = 10; row >= 6; row -= 2)
		{
			if (sb[row][col] == symbol && sb[row - 2][col + 5] == symbol)
				if (sb[row][col] == symbol && sb[row - 4][col + 10] == symbol)
					if (sb[row][col] == symbol && sb[row - 6][col + 15] == symbol)
						printf("%c won!\n\n", symbol);
			{
				if (sb[row][col] == symbol && sb[row - 2][col + 5] == symbol)
					if (sb[row][col] == symbol && sb[row - 4][col + 10] == symbol)
						if (sb[row][col] == symbol && sb[row - 6][col + 15] == symbol)
							menu();
			}
		}
	}
}
void checkWinner(char symbol, char sb[][Board_Hor])			//(MYOWN)
{
	//This function is a combination of all the check win functions
	//This is the function that is called everytime in switch_turn and play_computer
	//to see if there is a winner
	checkwin_vert(symbol, sb);
	checkwin_hor(symbol, sb);
	checkwin_leftacross(symbol, sb);
	checkwin_rightacross(symbol, sb);
}
//This allows one player to play against a computer. The computer will randomly generate letters
//A-G which will constitute as it's turn.
void play_computer(char player_1name[], char sb[][Board_Hor])
{
	int i, r;
	int gameover = 0;
	char answer, color;
	while (!gameover)
	{
		for (int i = 0; i < 2; i++)
		{
			do {
				if (i == 0)
				{
					printf("User: Please enter a letter A-G\n");
					scanf(" %c", &answer);
					color = 'R';
					//The computer will randomly generate a letter a-g which will play a piece for it
				}
				else {
					color = 'B';
					//This is the randomly generated number that the computer creates
					computer_turn(&answer);
				}
				switch (answer) //switch statement to allow the users to switch turns and play the game
				{
				case 'A':
				case 'a': play_piece(3, color, sb);
					break;
				case 'B':
				case 'b': play_piece(8, color, sb);
					break;
				case 'C':
				case 'c': play_piece(13, color, sb);
					break;
				case 'D':
				case 'd': play_piece(18, color, sb);
					break;
				case 'E':
				case 'e': play_piece(23, color, sb);
					break;
				case 'F':
				case 'f': play_piece(28, color, sb);
					break;
				case 'G':
				case 'g': play_piece(33, color, sb);
					break;
				default: printf("invalid choice\n");
					//end switch
				}

				//If player does not enter a value inbetween A and G, it will continue to ask for the correct value
			} while (!(answer == 'A' || answer == 'a' || answer == 'B' || answer == 'b' || answer == 'C' || answer == 'c' || answer == 'D' || answer == 'd' || answer == 'E' || answer == 'e' || answer == 'F' || answer == 'f' || answer == 'G' || answer == 'g'));
			display(sb);
			checkWinner(color, sb);
		}
	}
}
void menu_choiceC(char player_1name[], char sb[][Board_Hor])
{
	char junk;
	printf("Welcome to connect four\n");
	//printf("Please enter your name: \n");
	//fscanf("%c", &junk);
	//fgets(player_1name, nameLength, stdin);
	clearbuffer(sb);
	setBoard(sb);
	display(sb);
	//This function will allow one person to enter their name a play a computer
	play_computer(player_1name, sb);
}
void computer_turn(char *answer)				//(*PARAM)
{
	int r;
	//This line of code will randomly generate a letter a-g
	r = rand() % 7;									//		(RAND)
													//The pointer carries over the answer into my play_computer function
	*answer = 'a' + r;
}
int get_max_range(scoreType highscores[], int first, int last)			//(SORT)
{
	int i;
	int index_of_max;
	index_of_max = first;
	/*for each value of i from first+1 to last*/
	for (i = first + 1; i <= last; i++)
		/*if list[i] is greater than list[index_of_max]*/
		if (strcmp(highscores[i].names, highscores[index_of_max].names) > 0)
			/*update index_of_max to i*/
			index_of_max = i;

	return index_of_max;
}
void select_sort(scoreType highscores[], int n)
{
	int to_fill;
	int index_of_max;
	for (to_fill = n - 1; to_fill >= 1; to_fill--)
	{
		/*Find position of greatest element in unsorted array.*/
		index_of_max = get_max_range(highscores, 0, to_fill);
		/*Swap it with the element at end of unsorted array.*/
		swapscoretypes(highscores[index_of_max], highscores[to_fill]);
	}  /* end for to_fill */
}
void swapscoretypes(scoreType x, scoreType y)
{
	scoreType temp;
	//the swap function will switch the order of scores on my input file
	temp = x;
	x = y;
	y = temp;
}