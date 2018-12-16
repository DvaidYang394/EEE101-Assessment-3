/*************************************************************************************
 * Copyright (C) 2018
 * @File name: 1718112_3.c
 * @Author: Ziqi Yang
 * @Version: 6.1.0
 * @Date: 2018-12-16
 * @Description: EEE101-Assessment-3 Project
 *				 A game of rock, scissors and paper for user to against computer.
*************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>				/* Cal getch() function. */

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}		/* Get the length of array. */
#define rounds_X_text_pos 13				/* Coordinates for some characters in console. */
#define rounds_Y_text_pos 29
#define rounds_Y_character_pos 14
#define rounds_X_current_result_pos 35
#define rounds_Y_current_result_pos 31
#define rounds_user_X_pos 50
#define X_LENGTH 100						/* The width of console. */

typedef enum
{
	result_OK = 0,							/* The input is legal. */
	result_Error							/* The input is illegal. */
} General_Result;

typedef enum
{
	mini = 0,								/* Print mini size character picture. */
	normal									/* Print normal size character picture. */
} Character_Size;

typedef enum
{
	computer = 0,							/* Winner is computer. */
	player,									/* Winner is user. */
	none									/* Nobody win. */
} Game_Player;

typedef enum
{
	rock = 0,								/* Rock selected. */
	scissors,								/* Scissors selected. */
	paper									/* Paper selected. */
} General_Select;

typedef struct
{
	char			detail[256];			/* Name string. */
	General_Result	result;					/* Name input result. */
} name_info;

typedef struct
{
	char			detail[256];			/* Password string. */
	General_Result	result;					/* Password input result. */
} passwd_info;

typedef struct
{
	char			name[256];				/* File name. */
	FILE			*pointer;				/* Pointer of file. */
} file_info;

typedef struct
{
	int				games;					/* Times of games. */
	int				target;					/* Target games. */
	int				win;					/* Win times. */
	int				draw;					/* Draw times. */
	int				lose;					/* Lose times. */
	General_Result	result;					/* Games times input result. */
} times_info;

typedef struct
{
	name_info		name;					/* User name string. */
	passwd_info		passwd;					/* User password string. */
	times_info		times;					/* User times information. */
	int				final_win;				/* Whether the user win or not. */
	int				**record;				/* Game history. */
	int				login;					/* Whether log in or not. */
	file_info		file;					/* File information. */
} user_info;

/* Function declaration Start. */
char welcome_UI(void);
user_info login_UI(user_info user);
user_info signup_UI(user_info user);
char menu_UI(void);
user_info times_UI(user_info user);
General_Select computer_select_get(void);
General_Select user_select_get(void);
user_info rounds_UI(user_info user);
Game_Player compare(General_Select computer_select, General_Select user_select);
void final_UI(user_info user);
void datasave(user_info user);
void review_UI(user_info user);
void clear_UI(user_info user);
void printf_position(char *data, int init_X, int init_Y);
void printf_delta(char *data, int delta_X, int delta_Y);
void print_rock(Character_Size size, int bias_X, int bias_Y);
void print_scissors(Character_Size size, int bias_X, int bias_Y);
void print_paper(Character_Size size, int bias_X, int bias_Y);
void passwd_get(char *passwd);
/* Function declaration End. */

int main(void)
{
	char welcome_choice, menu_choice;
	user_info user = { {0,0},{0,0},{0,0,0,0,0,0},0,0,0,{0,0} };

	system("mode con cols=100 lines=34");					/* Set the width of console is 100 and height is 34. */
	system("color 1f");										/* Set the color of console background in blue and font in white. */
	do
	{
		welcome_choice = welcome_UI();						/* Welcome UI. */
		if (welcome_choice == 'a')
		{
			user = login_UI(user);							/* Log in UI. */
			while (user.login == 1)
			{
				menu_choice = menu_UI();					/* Menu UI. */
				if (menu_choice == 'a')
				{
					user = times_UI(user);					/* Get play times UI. */
					user = rounds_UI(user);					/* Game rounds UI. */
					final_UI(user);							/* Game result UI. */
				}
				else if (menu_choice == 'b')
					review_UI(user);						/* History review UI. */
				else if (menu_choice == 'c')
					clear_UI(user);							/* Clear history UI. */
				else
					user.login = 0;
			}
		}
		else if (welcome_choice == 'b')
			user = signup_UI(user);							/* Sign up UI. */
	} while (welcome_choice != 'c');
	printf_delta("The game will exit...", 39, 1);
	Sleep(1000);
	return 0;												/* Programm run successfully. */
}

char welcome_UI(void)										/* Welcome UI of the game. */
{
	char user_choice = 0;									/* Declare user_choice array to store user choice string. */
	int i = 0;												/* Declare i uses as run times of for loop. */
	General_Result user_result = result_Error;				/* Declare user_result to store the option input result. */

	system("cls");
	printf_position("Welcome to the EXCITING GAME!\n", 34, 0);
	print_rock(mini, 21, 0);
	print_scissors(mini, 40, 0);
	print_paper(mini, 57, 0);
	printf_position("Please choose the option with serial number before it.\n", 22, 9);
	printf_delta("a. Log in your account.\n", 38, 1);
	printf_delta("b. Create a new account.\n", 38, 1);
	printf_delta("c. EXIT.\n", 38, 1);

	while (user_result == result_Error)						/* When user input is illegal. */
	{
		rewind(stdin);
		user_choice = getch();
		if (user_choice != '\0')
		{
			switch (user_choice)							/* Judge the user choice. */
			{
			case 'a':
			case 'b':
			case 'c':
				user_result = result_OK;					/* The choice is 'a', 'b' or 'c', input is legal. */
				break;
			default:
				user_result = result_Error;					/* The choice is others, input is illegal. */
			}
		}
	}
	return user_choice;
}

user_info login_UI(user_info user)			/* Log in UI of the game. */
{
	int i = 0, j = 0;
	char target_user_passwd[256] = { 0 };
	char inputc = 0;

	user.name.result = result_Error;
	user.passwd.result = result_Error;

	while (user.name.result == result_Error)
	{
		system("cls");
		printf("Please input your user name(log in): ");
		rewind(stdin);
		gets(user.name.detail);

		strcpy(user.file.name, user.name.detail);
		strcat(user.file.name, ".txt");
		user.file.pointer = fopen(user.file.name, "r");
		if (user.file.pointer == NULL)									/* If the user doesn't exist, remind the user. */
		{
			printf("\nThe user does not exist, please try again!\n");
			Sleep(1500);
		}
		else
			user.name.result = result_OK;
	}

	while (user.passwd.result == result_Error)							/* Input and check the password. */
	{
		user.passwd.result = result_OK;
		printf_position("Please input your password: ", 0, 2);
		passwd_get(user.passwd.detail);

		fseek(user.file.pointer, strlen(user.name.detail) + 1, SEEK_SET);
		fscanf(user.file.pointer, "%s", target_user_passwd);

		if (strcmp(user.passwd.detail,target_user_passwd) != 0)
			user.passwd.result = result_Error;

		if (user.passwd.result == result_Error)							/* If the password is incorrect. */
		{
			printf("\nYour password is incorrect, please try again!\n");
			Sleep(1500);
			printf_position("", 0, 2);
			for (i = 0; i < 5; i++)
			{
				for (j = 0; j < X_LENGTH; j++)
					printf(" ");
				printf("\n");
			}
			printf_delta("", 0, -6);
		}
	}
	fclose(user.file.pointer);
	printf("\nLog in successfully, please wait for a few seconds...");
	user.login = 1;														/* Update the log in state. */
	Sleep(1500);
	return user;
}

user_info signup_UI(user_info user)										/* Sign up UI of the game. */
{
	int i = 0, j = 0;
	char inputc = 0;

	user.name.result = result_Error;
	user.passwd.result = result_Error;

	while (user.name.result == result_Error)
	{
		system("cls");
		printf("Please input your user name(sign up): ");
		rewind(stdin);
		gets(user.name.detail);

		if (strlen(user.name.detail) != 0)
		{
			for (i = 0; i < strlen(user.name.detail); i++)				/* Check if the name is space. */
			{
				if (user.name.detail[i] != 32)
				{
					user.name.result = result_OK;						/* The name is not space, input legal. */
					printf("\n");
					break;
				}
			}
		}

		if (user.name.result == result_OK)
		{
			strcpy(user.file.name, user.name.detail);
			strcat(user.file.name, ".txt");
			user.file.pointer = fopen(user.file.name, "r");
			if (user.file.pointer != NULL)								/* If the user has exist, remind the user. */
			{
				printf("The user has already exists, please try again!\n");
				Sleep(1500);
				fclose(user.file.pointer);
				user.name.result = result_Error;
				continue;
			}
		}

		if (user.name.result == result_Error)							/* If the name input is illegal(name is space). */
		{
			printf("\nThe name cannot be space, please try again!");
			Sleep(1500);
		}
	}

	while (user.passwd.result == result_Error)
	{
		printf_position("Please input your password(must between 6 and 15 digits and cannot include space): ", 0, 2);
		passwd_get(user.passwd.detail);

		if (strlen(user.passwd.detail) >= 6 && strlen(user.passwd.detail) <= 15)
		{
			user.passwd.result = result_OK;
			for (i = 0; i < strlen(user.passwd.detail); i++)			
			{
				if (user.passwd.detail[i] == 32)						/* If the password has space, input illegal. */
				{
					user.passwd.result = result_Error;
					printf("\n");
					break;
				}
			}
		}
		if (user.passwd.result == result_Error)							/* If the password input is illegal. */
		{
			printf("\nYour password is illegal, please try again!\n");
			Sleep(1500);
			printf_position("", 0, 2);
			for (i = 0; i < 5; i++)
			{
				for (j = 0; j < X_LENGTH; j++)
					printf(" ");
				printf("\n");
			}
			printf_delta("", 0, -6);
		}
	}
	user.file.pointer = fopen(user.file.name, "w+");
	fprintf(user.file.pointer, "%s,%s\n%d\n", user.name.detail, user.passwd.detail, user.times.games);		
																		/* Write user information to the file. */
	rewind(user.file.pointer);
	fclose(user.file.pointer);
	printf("\nAccount created successfully!");
	Sleep(1000);
	return user;
}

char menu_UI(void)
{
	char user_choice = 0;
	int i = 0;
	General_Result user_result = result_Error;

	system("cls");
	printf_position("Rock, Scissors and Paper GAME!\n\n", 35, 0);
	printf_position("a. Start a new game.", 38, 3);
	printf_position("b. Review game history.", 38, 5);
	printf_position("c. Clear game history.", 38, 7);
	printf_position("d. Logout.", 38, 9);

	while (user_result == result_Error)						/* When user input is illegal. */
	{
		rewind(stdin);
		user_choice = getch();
		if (user_choice != '\0')
		{
			switch (user_choice)							/* Judge the user choice. */
			{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
				user_result = result_OK;
				break;
			default:
				user_result = result_Error;
			}
		}
	}
	return user_choice;
}

user_info times_UI(user_info user)								/* Information input UI of the game. */
{
	int i = 0;
	char user_times[256] = { 0 };
	user.times.result = result_Error;

	while (user.times.result == result_Error)
	{
		system("cls");
		user.times.result = result_OK;							/* Pretend the times input is legal at first. */
		printf("Enter the times you want to play(must be a POSITIVE INTEGER and SMALLER THAN 50!): ");
		rewind(stdin);
		gets(user_times);

		if (strlen(user_times) != 0)
		{
			for (i = 0; i < strlen(user_times); i++)			/* Check if the times input is number. */
			{
				if (!isdigit(user_times[i]))
				{
					user.times.result = result_Error;
					break;
				}
			}
		}
		if (user.times.result == result_OK)						/* Check if the times is less than 50. */
		{
			user.times.target = atoi(user_times);
			if (user.times.target <= 0 || user.times.target >= 50) 
				user.times.result = result_Error;
		}
		if (user.times.result == result_Error)
		{
			printf("\nThe times you input is illegal, please try again!\n");
			Sleep(1500);
		}
	}
	return user;
}

General_Select computer_select_get(void)					/* Generate and print computer select result. */
{
	int randnum = 0;										/* Declare randnum to store random number. */
	General_Select computer_select;							/* Declare computer_select to store the character computer select. */

	srand((unsigned int)time(NULL));						/* Generate a seed with time to produce a random number. */
	randnum = rand() % 3;									/* Produce a random number among 0, 1, 2. */
	switch (randnum)
	{
	case 0:
		computer_select = rock;								/* Check and print computer character. */
		print_rock(normal, 0, rounds_Y_character_pos);
		break;
	case 1:
		computer_select = scissors;
		print_scissors(normal, 0, rounds_Y_character_pos);
		break;
	default:
		computer_select = paper;
		print_paper(normal, 0, rounds_Y_character_pos);
	}
	return computer_select;									/* Return the character computer selected. */
}

General_Select user_select_get(void)						/*Choose and print user select result.  */
{
	char select_input[256] = { 0 };	
	int i = 0;
	General_Result select_result = result_Error;
	General_Select user_select;

	printf("Please use a letter to choose what you want, the meanings are as follows:\n");
	printf("r: Rock \t\t s: Scissors \t\t p: Paper\n\n");

	while (select_result == result_Error)
	{
		printf_position("Computer choice is:", 0, 13);
		printf_delta("Your choice is(r/s/p): ", 30, 0);
		rewind(stdin);
		gets(select_input);

		if (strlen(select_input) == 1)
		{
			switch (select_input[0])
			{
			case 'r':
			case 's':
			case 'p':
				select_result = result_OK;					/* The input is legal. */
				break;
			default:
				select_result = result_Error;				/* The input is illegal. */
			}
		}
		if (select_result == result_Error)
		{
			printf("\nYour input is illegal, please try again!\n");
			Sleep(1500);
			printf_delta("", 0, -3);
			for (i = 0; i < X_LENGTH; i++)
				printf(" ");
			printf("\n\n");
			for (i = 0; i < X_LENGTH; i++)
				printf(" ");
		}
	}

	switch (select_input[0])
	{
	case 'r':
		user_select = rock;														/* Assigning user select. */
		print_rock(normal, rounds_user_X_pos, rounds_Y_character_pos);			/* Print normal size of user select character. */
		break;
	case 's':
		user_select = scissors;
		print_scissors(normal, rounds_user_X_pos, rounds_Y_character_pos);
		break;
	default:
		user_select = paper;
		print_paper(normal, rounds_user_X_pos, rounds_Y_character_pos);
	}
	return user_select;															/* Return the character user selected. */
}

user_info rounds_UI(user_info user)							/* Rounds UI of the game. */
{
	int remain_games, i;
	General_Select computer_select, user_select;			/* Declare to store the character computer and user select. */
	Game_Player current_winner;								/* Declare to store the current and final winner of game. */

	user.times.win = 0;
	user.times.lose = 0;
	user.times.draw = 0;
	for (remain_games = user.times.target; remain_games > 0; remain_games--)		/* When the remain times of game is more than 0. */
	{
		system("cls");
		printf("(If you want, press Ctrl + C to exit immediately during the game.)\n\n");
		printf("You have %d times to play!\n\n", remain_games);
		printf_delta("Score List:\n\n", 40, 0);
		printf_delta("", 40, 0);
		printf("%s(YOU): %d\n\n", user.name.detail, user.times.win);
		printf_delta("", 40, 0);
		printf("COMPUTER: %d\n\n", user.times.lose);

		user_select = user_select_get();
		computer_select = computer_select_get();
		current_winner = compare(computer_select, user_select);
		if (current_winner == computer)
		{
			printf_position("Computer wins this time!\n", rounds_X_current_result_pos, rounds_Y_current_result_pos);
			user.times.lose++;									/* Add times of computer win. */
		}
		else if (current_winner == player)
		{
			printf_position("You win this time!\n", rounds_X_current_result_pos, rounds_Y_current_result_pos);
			user.times.win++;									/* Add times of user win. */
		}
		else
		{
			printf_position("Nobody wins this time!\n", rounds_X_current_result_pos, rounds_Y_current_result_pos);
			user.times.draw++;
		}

		printf_position("Press \"Enter\" to continue game...", 31, 33);
		rewind(stdin);
		while (getch() != '\r');								/* Get "Enter" to continue game. */
	}

	/* Compare the times of computer and user win and judge and final winner. */
	if (user.times.lose > user.times.win) user.final_win = 0;
	else if (user.times.lose < user.times.win) user.final_win = 1;
	else user.final_win = 2;
	return user;									/* Return the final winner of the game. */
}

Game_Player compare(General_Select computer_select, General_Select user_select)			/* Compare the character that computer and user select. */
{
	Game_Player current_winner;										/* Declare current_winner to store current winner. */

	if (computer_select == user_select) current_winner = none;		/* If the characters are same. Nobody win this time. */
	else
	{
		switch (computer_select)									/* Logic to judge the winner. */
		{
		case rock:
			if (user_select == rock) current_winner = none;
			if (user_select == scissors) current_winner = computer;
			if (user_select == paper) current_winner = player;
			break;
		case scissors:
			if (user_select == rock) current_winner = player;
			if (user_select == scissors) current_winner = none;
			if (user_select == paper) current_winner = computer;
			break;
		default:
			if (user_select == rock) current_winner = computer;
			if (user_select == scissors) current_winner = player;
			if (user_select == paper) current_winner = none;
		}
	}
	return current_winner;											/* Return the current winner. */
}

void final_UI(user_info user)								/* Final UI of the game. */
{
	General_Result user_result = result_Error;
	char user_choice[256] = { 0 };
	int i = 0;

	system("cls");
	switch (user.final_win)									/* Print different sentences with different final winner. */
	{
	case 0:
		printf_delta("Sorry, you lose :(\n", 41, 0);
		break;
	case 1:
		printf_delta("Congratulations! You win :)\n", 36, 0);
		break;
	default:
		printf_delta("Nobody win the game =_=\n", 38, 0);
	}
	printf_delta("Your Score List:\n", 41, 1);
	printf_delta("", 46, 1);
	printf("Win: %d\n", user.times.win);
	printf_delta("", 46, 1);
	printf("Lose: %d\n", user.times.lose);
	printf_delta("", 46, 1);
	printf("Draw: %d\n", user.times.draw);
	printf_delta("Press \"Enter\" to back to menu...", 34, 1);
	while (getch() != '\r');
	datasave(user);											/* Call datasave function to save the game data to the history in the file. */
}

void datasave(user_info user)								/* Save game data. */
{
	int i = 0, j = 0;
	char temp = 0;
	long times_pos = 0;
	user.file.pointer = fopen(user.file.name, "r+");
	fseek(user.file.pointer, strlen(user.name.detail) + strlen(user.passwd.detail) + 3, SEEK_SET);
	times_pos = ftell(user.file.pointer);
	fscanf(user.file.pointer, "%d", &user.times.games);		/* Read the value of game times. */
	user.times.games++;										/* Game times add by 1. */
	fseek(user.file.pointer, times_pos, SEEK_SET);
	fprintf(user.file.pointer, "%d", user.times.games);		/* Change the value of game times. */
	user.record = malloc(1 * sizeof(int));					/* Creat a dynamic array. */
	user.record[0] = malloc(6 * sizeof(int));

	for (i = 0; i < 6; i++)
		user.record[0][i] = 0;

	fseek(user.file.pointer, 0, SEEK_END);
	user.record[0][0] = user.times.games;					/* Update the data of last game to the array. */
	user.record[0][1] = user.times.target;
	user.record[0][2] = user.times.win;
	user.record[0][3] = user.times.lose;
	user.record[0][4] = user.times.draw;
	user.record[0][5] = user.final_win;
	for (j = 0; j < 6; j++)									/* Update the data of last game to the file. */
	{
		if (j < 5)
			fprintf(user.file.pointer, "%d\t", user.record[0][j]);
		else
			fprintf(user.file.pointer, "%d\n", user.record[0][j]);
	}
	rewind(user.file.pointer);
	fclose(user.file.pointer);
	free(user.record[0]);									/* Free. */
	free(user.record);
}

void review_UI(user_info user)								/* Review game history. */
{
	int i, j;
	char temp = 0;

	system("cls");
	printf_position("Game History\n", 41, 1);
	user.file.pointer = fopen(user.file.name, "r+");
	fseek(user.file.pointer, strlen(user.name.detail) + strlen(user.passwd.detail) + 3, SEEK_SET);
	fscanf(user.file.pointer, "%d", &user.times.games);

	user.record = malloc(user.times.games * sizeof(int));	/* Create a dynamic array according to the history game times. */
	for (i = 0; i < user.times.games; i++)
		user.record[i] = malloc(6 * sizeof(int));
	
	for (i = 0; i < user.times.games; i++)
		for (j = 0; j < 6; j++)
			user.record[i][j] = 0;

	fseek(user.file.pointer, 2, SEEK_CUR);
	for (i = 0; i < user.times.games; i++)					/* Read the game history. */
	{
		for (j = 0; j < 6; j++)
			fscanf(user.file.pointer, "%d", &user.record[i][j]);
		while (1)
		{
			fscanf(user.file.pointer, "%c", &temp);
			if (temp == '\n') break;
		}
	}
	fclose(user.file.pointer);

	/* Print the game history. */
	printf_position("Games\tTotal\tWin\tLose\tDraw\tResult\n", 24, 3);
	printf_delta("", 24, 0);
	for (i = 0; i < 46; i++)
		printf("-");
	printf("\n");
	for (i = 0; i < user.times.games; i++)
	{
		printf_delta("", 24, 0);
		for (j = 0; j < 5; j++)
			printf(" %d\t", user.record[i][j]);
		switch (user.record[i][5])
		{
		case 0: printf("Lose"); break;
		case 1: printf("Win"); break;
		case 2: printf("Draw"); break;
		}
		printf("\n");
	}
	for (i = 0; i < user.times.games; i++)
		free(user.record[i]);
	free(user.record);

	printf_delta("Press \"Enter\" to back to menu...", 31, 2);
	while (getch() != '\r');								/* The method to return the menu. */
}

void clear_UI(user_info user)								/* Clear the game history. */
{
	char user_choice = 0;
	int i = 0;
	General_Result user_result = result_Error;

	system("cls");
	printf_position("Would you really like to clear game history(y/n)?\n", 25, 0);
	printf_delta("WARNING: This operation is irrevocable!\n", 30, 1);
	printf_delta("Press \"y\" to clear or \"n\" to back to the menu.\n", 27, 1);

	while (user_result == result_Error)						/* When user input is illegal. */
	{
		rewind(stdin);
		user_choice = getch();
		if (user_choice != '\0')
		{
			switch (user_choice)
			{
			case 'y':
			case 'n':
				user_result = result_OK;					/* The choice is 'y' or 'n', input is legal. */
				break;
			default:
				user_result = result_Error;					/* The choice is others, input is illegal. */
			}
		}
	}

	if (user_choice == 'y')
	{
		/* Rewrite and init the user file. */
		user.times.games = 0;
		user.file.pointer = fopen(user.file.name, "w+");
		fprintf(user.file.pointer, "%s,%s\n%d\n", user.name.detail, user.passwd.detail, user.times.games);
		rewind(user.file.pointer);
		fclose(user.file.pointer);

		printf_delta("Clear successfully!", 40, 1);
		Sleep(1500);
	}
}

void printf_position(char *data, int init_X, int init_Y)	/* Printf on specific position. */
{
	HANDLE hd;												/* Declare hd to store handle to standard output. */
	COORD position;											/* Declare position to store coordinates. */

	hd = GetStdHandle(STD_OUTPUT_HANDLE);					/* Get handle to standard output. */
	position.X = init_X;									/* Set the x-axis coordinates. */
	position.Y = init_Y;									/* Set the y-axis coordinates. */

	SetConsoleCursorPosition(hd, position);					/* Move the cursor to the set position of console. */
	printf("%s", data);										/* Print the data. */
}

void printf_delta(char *data, int delta_X, int delta_Y)		/* Printf on incremental position. */
{
	HANDLE hd;
	COORD position;
	CONSOLE_SCREEN_BUFFER_INFO console_buf;					/* Declare to store buffer info of console. */

	hd = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hd, &console_buf);			/* Get buffer info of console. */
	position.X = console_buf.dwCursorPosition.X;			/* Get current x-axis coordinates of cursor. */
	position.Y = console_buf.dwCursorPosition.Y;			/* Get current y-axis coordinates of cursor. */
	position.X += delta_X;
	position.Y += delta_Y;
	
	SetConsoleCursorPosition(hd, position);					/* Move the cursor to the set position of console. */
	printf("%s", data);
}

void print_rock(Character_Size size, int bias_X, int bias_Y)	/* Print the picture of rock. */
{
	/* Declare x_normal, y_normal, x_mini, y_mini to store the coordinates of points in the picture. */
	int x_normal[] = { 6,7,8,5,6,7,8,9,4,5,6,7,8,9,10,3,4,5,6,7,8,9,10,11,2,3,4,5,6,7,8,9,10,11,12,2,3,4,5,6,7,8,9,10,11,12,12,11,10,9,8,7,6,5,4,3,2,11,10,9,8,7,6,5,4,3,10, 9, 8, 7, 6, 5, 4, 9, 8, 7, 6, 5, 8, 7, 6 };
	int y_normal[] = { 2,2,2,3,3,3,3,3,4,4,4,4,4,4, 4,5,5,5,5,5,5,5, 5, 5,6,6,6,6,6,6,6,6, 6, 6, 6,7,7,7,7,7,7,7,7, 7, 7, 7, 8, 8, 8,8,8,8,8,8,8,8,8, 9, 9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12 };
	int x_mini[] = { 4,5,3,4,5,6,2,3,4,5,6,7,7,6,5,4,3,2,6,5,4,3,5,4 };
	int y_mini[] = { 2,2,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,7,7 };
	int pos_len, i;											/* Declare pos_len to store the array length.
															   Declare i uses as run times of for loop. */

	if (size == normal)										/* Print normal size of the picture. */
	{
		GET_ARRAY_LEN(x_normal, pos_len);					/* Get the length of the array. */
		for (i = 0; i < pos_len; i++)
			printf_position("*", bias_X + x_normal[i] * 2, bias_Y + y_normal[i]);
		printf_position("Rock", bias_X + rounds_X_text_pos, rounds_Y_text_pos);
	}
	else													/* Print mini size of the picture. */
	{
		GET_ARRAY_LEN(x_mini, pos_len);
		for (i = 0; i < pos_len; i++)
			printf_position("*", bias_X + x_mini[i] * 2, bias_Y + y_mini[i]);
		printf("\n");
	}
}

void print_scissors(Character_Size size, int bias_X, int bias_Y)	/* Print the picture of scissors. */
{
	/* Declare x_normal, y_normal, x_mini, y_mini to store the coordinates of points in the picture. */
	int x_normal[] = { 3,4,11,2,5,10,3,5,9,4,8,5,7,6,7,5,8,4, 9, 5, 3,10, 5, 2,11, 4, 3 };
	int y_normal[] = { 2,2, 2,3,3, 3,4,4,4,5,5,6,6,7,8,8,9,9,10,10,10,11,11,11,12,12,12 };
	int x_mini[] = {2,3,6,2,3,5,4,5,3,2,6,3,2};
	int y_mini[] = {2,2,2,3,3,3,4,5,5,5,6,6,6};
	int pos_len, i;											/* Declare pos_len to store the array length.
															   Declare i uses as run times of for loop. */

	if (size == normal)
	{
		GET_ARRAY_LEN(x_normal, pos_len);
		for (i = 0; i < pos_len; i++)
			printf_position("*", bias_X + x_normal[i] * 2.5, bias_Y + y_normal[i]);
		printf_position("Scissors", bias_X + rounds_X_text_pos, rounds_Y_text_pos);
	}
	else
	{
		GET_ARRAY_LEN(x_mini, pos_len);
		for (i = 0; i < pos_len; i++)
			printf_position("*", bias_X + x_mini[i] * 2, bias_Y + y_mini[i]);
		printf("\n");
	}
}

void print_paper(Character_Size size, int bias_X, int bias_Y)		/* Print the picture of paper. */
{
	/* Declare x_normal, y_normal, x_mini, y_mini to store the coordinates of points in the picture. */
	int x_normal[] = { 2,2,2,2,2,2,2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,2,2 };
	int y_normal[] = { 2,3,4,5,6,7,8,9,10,11,12 };
	int x_mini[] = { 2,2,2,2,2,2,2,2,2,2,2,2 };
	int y_mini[] = { 2,3,4,5,6,7 };
	int pos_x_len, pos_y_len, i, j;							/* Declare pos_x_len, pos_y_len to store the array length.
															   Declare i, j use as run times of for loop. */

	if (size == normal)
	{
		GET_ARRAY_LEN(x_normal, pos_x_len);
		GET_ARRAY_LEN(y_normal, pos_y_len);
		for (i = 0; i < pos_y_len; i++)
		{
			printf_position("", bias_X + x_normal[i] * 2.5, bias_Y + y_normal[i]);
			for (j = 0; j < pos_x_len; j++)
				printf("~");
		}
		printf_position("Paper", bias_X + rounds_X_text_pos, rounds_Y_text_pos);
	}
	else
	{
		GET_ARRAY_LEN(x_mini, pos_x_len);
		GET_ARRAY_LEN(y_mini, pos_y_len);
		for (i = 0; i < pos_y_len; i++)
		{
			printf_position("", bias_X + x_normal[i] * 2, bias_Y + y_normal[i]);
			for (j = 0; j < pos_x_len; j++)
				printf("~");
		}
		printf("\n");
	}
}

void passwd_get(char *passwd)								/* Get the password in secret character(*). */
{
	char inputc = 0;										/* Receive the character. */
	int i = 0;
	for (i = 0; i < 256; i++)
		passwd[i] = 0;
	i = 0;
	rewind(stdin);
	while (inputc != '\r')
	{
		inputc = getch();
		if (inputc != '\r' && inputc != '\0')				/* The input is not enter or \0 */
		{
			if (inputc != '\b')								/* The input is not backspace. */
			{
				passwd[i] = inputc;							/* Character correct, record to the array. */
				printf("*");
				i += 1;
			}
			else											/* Delete last character. */
			{
				if (i > 0)
				{
					i -= 1;
					passwd[i] = '\0';
					printf("\b");
					printf("%c", passwd[i]);
					printf("\b");
				}
				else
					i = 0;
			}
		}
	}
	printf("\n");
	return passwd;
}