/*************************************************************************************
 * Copyright (C) 2018
 * @File name: 1718112_3.c
 * @Author: Ziqi Yang
 * @Version: 1.0.0
 * @Date: 2018-11-25
 * @Description: EEE101-Assessment-3 Project
 *				 A game of rock, scissors and paper for user to against computer.
*************************************************************************************/

/**
 * @brief	libraries for the project.
 * @date	2018-11-2	
 */
#include <stdio.h>		/* For use of printf(), gets(), rewind(), sizeof(), getchar(). */
#include <string.h>		/* For use of strlen(). */
#include <Windows.h>	/* For use of Sleep(), GetStdHandle(), GetConsoleScreenBufferInfo(), SetConsoleCursorPosition(). */
#include <ctype.h>		/* For use of isdigit(). */
#include <stdlib.h>		/* For use of system(), srand(), rand(). */
#include <time.h>		/* For use of time(). */

/**
 * @brief	define for the project.
 * @date	2018-11-2						
 */
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}		/* Get the length of array. */
#define rounds_X_text_pos 13				/* x-axis coordinates for character text in console in rounds UI. */
#define rounds_Y_text_pos 29				/* y-axis coordinates for character text in console in rounds UI. */
#define rounds_Y_character_pos 14			/* y-axis coordinates for character pictures in console in rounds UI. */
#define rounds_X_current_result_pos 35		/* x-axis coordinates for current result text in console in rounds UI. */
#define rounds_Y_current_result_pos 31		/* y-axis coordinates for current result text in console in rounds UI. */
#define rounds_user_X_pos 50				/* x-axis coordinates for user character picture in console in rounds UI. */
#define X_LENGTH 100						/* The width of console. */

/**
 * @enum	General_Result
 * @brief	a enum type to represent the input result.
 * @date	2018-11-2
 *
 * @var	result_OK		the input is legal.
 * @var	result_Error	the input is illegal.
 */
typedef enum
{
	result_OK = 0,
	result_Error
} General_Result;

/**
 * @enum	Character_Size
 * @brief	a enum type to represent the size of the character picture needed to print.
 * @date	2018-11-2
 *
 * @var	mini	print mini size character picture.
 * @var normal	print normal size character picture.
 */
typedef enum
{
	mini = 0,
	normal
} Character_Size;

/**
* @enum	Game_Player
* @brief	a enum type to represent the winner of game.
* @date	2018-11-2
*
* @var	computer	winner is computer.
* @var	user		winner is user.
* @var	none		nobody win.
*/
typedef enum
{
	computer = 0,
	player,
	none
} Game_Player;

/**
* @enum	General_Select
* @brief	a enum type to represent the selected character of computer or user.
* @date	2018-11-2
*
* @var	rock		rock selected.
* @var	scissors	scissors selected.
* @var	paper		paper selected.
*/
typedef enum
{
	rock = 0,
	scissors,
	paper
} General_Select;

typedef struct
{
	char			detail[256];
	General_Result	result;
} name_info;

typedef struct
{
	char			detail[256];
	General_Result	result;
} passwd_info;

typedef struct
{
	char			name[256];
	FILE			*pointer;
} file_info;

typedef struct
{
	int				games;
	int				target;
	int				win;
	int				draw;
	int				lose;
	General_Result	result;
} times_info;

typedef struct
{
	name_info		name;
	passwd_info		passwd;
	times_info		times;
	int				final_win;
	int				**record;
	int				login;
	file_info		file;
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
/* Function declaration End. */

int main(void)
{
	char welcome_choice, menu_choice;
	user_info user = { {0,0},{0,0},{0,0,0,0,0,0},0,0,0,{0,0} };

	system("mode con cols=100 lines=34");					/* Set the width of console is 100 and height is 34. */
	system("color 1f");										/* Set the color of console background in blue and font in white. */
	do
	{
		welcome_choice = welcome_UI();
		if (welcome_choice == 'a')
		{
			user = login_UI(user);
			while (user.login == 1)
			{
				menu_choice = menu_UI();
				if (menu_choice == 'a')
				{
					user = times_UI(user);
					user = rounds_UI(user);
					final_UI(user);
				}
				else if (menu_choice == 'b')
					review_UI(user);
				else if (menu_choice == 'c')
					clear_UI(user);
				else
					user.login = 0;
			}
		}
		else if (welcome_choice == 'b')
			user = signup_UI(user);
	} while (welcome_choice != 'c');

	return 0;												/* Programm run successfully. */
}

/**
 * @name	welcome_UI
 * @brief	welcome UI of the game.
 * @date	2018-11-2
 *
 * @param	option_choice		the option user choose, which to decide start or exit game.
 */
char welcome_UI(void)
{
	char user_choice[256] = { 0 };							/* Declare user_choice array to store user choice string. */
	int i = 0;												/* Declare i uses as run times of for loop. */
	General_Result user_result = result_Error;				/* Declare user_result to store the option input result. */

	system("cls");
	printf_position("Welcome to the EXCITING GAME!\n",34,0);
	print_rock(mini, 21, 0);
	print_scissors(mini, 40, 0);
	print_paper(mini, 57, 0);
	printf_position("Please choose the option with serial number before it.\n",22,9);
	printf_delta("a. Log in your account.\n", 38, 1);
	printf_delta("b. Create a new account.\n", 38, 1);
	printf_delta("c. EXIT.\n", 38, 1);

	while (user_result == result_Error)						/* When user input is illegal. */
	{
		printf_position("Your choice is: ",42,17);
		rewind(stdin);
		gets(user_choice);
		rewind(stdin);

		if (strlen(user_choice) == 1)						/* If the input is 1 character. */
		{
			switch (user_choice[0])							/* Judge the user choice. */
			{
			case 'a':
			case 'b':
			case 'c':
				user_result = result_OK;					/* The choice is 'a' or 'b', input is legal. */
				break;
			default:
				user_result = result_Error;					/* The choice is others, input is illegal. */
			}
		}
		if (user_result == result_Error)					/* If the input is illegal. */
		{
			printf_delta("Your input is illegal, please try again!\n", 30, 1);
			Sleep(1500);									/* Wait for 1500 ms. */
			printf_delta("", 0, -3);						/* Change the position of cursor. */
			for (i = 0; i < X_LENGTH; i++)					/* Clear incorrect output on console. */
				printf(" ");
			printf("\n\n");
			for (i = 0; i < X_LENGTH; i++)
				printf(" ");
		}
	}

	if (user_choice[0] == 'c')
	{
		printf_delta("The game will exit...", 34, 1);
		Sleep(1000);
	}

	return user_choice[0];			/* Change the value on address option_choice to user_choice[0](to use in main function). */
}

user_info login_UI(user_info user)
{
	int i = 0, j = 0;
	char target_user_passwd[256] = { 0 };

	user.name.result = result_Error;
	user.passwd.result = result_Error;

	while (user.name.result == result_Error)
	{
		system("cls");
		printf("Please input your user name: ");
		rewind(stdin);
		gets(user.name.detail);
		rewind(stdin);

		for (i = 0; i < 256; i++)
			user.file.name[i] = user.name.detail[i];
		strcat(user.file.name, ".txt");
		user.file.pointer = fopen(user.file.name, "r");
		if (user.file.pointer == NULL)						/* 遗留大小写无法区分的bug */
		{
			printf("\nThe user does not exist, please try again!\n");
			Sleep(1500);
		}
		else
			user.name.result = result_OK;
	}

	while (user.passwd.result == result_Error)
	{
		user.passwd.result = result_OK;
		printf_position("Please input your password: ", 0, 2);
		rewind(stdin);
		gets(user.passwd.detail);
		rewind(stdin);

		fseek(user.file.pointer, strlen(user.name.detail) + 1, SEEK_SET);
		fscanf(user.file.pointer, "%s", target_user_passwd);
		if (strlen(user.passwd.detail) != strlen(target_user_passwd))
			user.passwd.result = result_Error;
		else
		{
			for (i = 0; i < strlen(user.passwd.detail); i++)
			{
				if (user.passwd.detail[i] != target_user_passwd[i])
				{
					user.passwd.result = result_Error;
					break;
				}
			}
		}

		if (user.passwd.result == result_Error)					/* If the name input is illegal(name is space). */
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
	user.login = 1;
	Sleep(1500);
	return user;
}

user_info signup_UI(user_info user)
{
	int i = 0, j = 0;

	user.name.result = result_Error;
	user.passwd.result = result_Error;

	while (user.name.result == result_Error)
	{
		system("cls");
		printf("Please input your user name: ");
		rewind(stdin);
		gets(user.name.detail);
		rewind(stdin);

		if (strlen(user.name.detail) != 0)
		{
			for (i = 0; i < strlen(user.name.detail); i++)			/* Check if the name is space. */
			{
				if (user.name.detail[i] != 32)
				{
					user.name.result = result_OK;				/* The name is not space, input legal. */
					printf("\n");
					break;
				}
			}
		}

		if (user.name.result == result_OK)
		{
			for (i = 0; i < 256; i++)
				user.file.name[i] = user.name.detail[i];
			strcat(user.file.name, ".txt");
			user.file.pointer = fopen(user.file.name, "r");
			if (user.file.pointer != NULL)
			{
				printf("The user has already exists, please try again!\n");
				Sleep(1500);
				fclose(user.file.pointer);
				user.name.result = result_Error;
				continue;
			}
		}

		if (user.name.result == result_Error)					/* If the name input is illegal(name is space). */
		{
			printf("\nThe name cannot be space, please try again!");
			Sleep(1500);
		}
	}

	while (user.passwd.result == result_Error)
	{
		printf_position("Please input your password(must between 6 and 15 digits and cannot include space): ", 0, 2);
		rewind(stdin);
		gets(user.passwd.detail);
		rewind(stdin);

		if (strlen(user.passwd.detail) >= 6 && strlen(user.passwd.detail) <= 15)
		{
			user.passwd.result = result_OK;
			for (i = 0; i < strlen(user.passwd.detail); i++)			/* Check if the name is space. */
			{
				if (user.passwd.detail[i] == 32)
				{
					user.passwd.result = result_Error;				/* The name is not space, input legal. */
					printf("\n");
					break;
				}
			}
		}
		if (user.passwd.result == result_Error)					/* If the name input is illegal(name is space). */
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
	rewind(user.file.pointer);
	fclose(user.file.pointer);
	printf("\nAccount created successfully!");
	Sleep(1000);
	return user;
}

char menu_UI(void)
{
	char user_choice[256] = { 0 };							/* Declare user_choice array to store user choice string. */
	int i = 0;												/* Declare i uses as run times of for loop. */
	General_Result user_result = result_Error;				/* Declare user_result to store the option input result. */

	system("cls");
	printf_position("Rock, Scissors and Paper GAME!\n\n", 35, 0);
	printf_position("a. Start a new game.", 38, 3);
	printf_position("b. Review game history.", 38, 5);
	printf_position("c. Clear game history.", 38, 7);
	printf_position("d. Logout.", 38, 9);
	
	while (user_result == result_Error)						/* When user input is illegal. */
	{
		printf_position("Your choice is: ", 39, 11);
		rewind(stdin);
		gets(user_choice);
		rewind(stdin);

		if (strlen(user_choice) == 1)						/* If the input is 1 character. */
		{
			switch (user_choice[0])							/* Judge the user choice. */
			{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
				user_result = result_OK;					/* The choice is 'a' or 'b', input is legal. */
				break;
			default:
				user_result = result_Error;					/* The choice is others, input is illegal. */
			}
		}
		if (user_result == result_Error)					/* If the input is illegal. */
		{
			printf_delta("Your input is illegal, please try again!\n", 30, 1);
			Sleep(1500);									/* Wait for 1500 ms. */
			printf_delta("", 0, -3);						/* Change the position of cursor. */
			for (i = 0; i < X_LENGTH; i++)					/* Clear incorrect output on console. */
				printf(" ");
			printf("\n\n");
			for (i = 0; i < X_LENGTH; i++)
				printf(" ");
		}
	}

	return user_choice[0];
}

/**
* @name	info_input
* @brief	information input UI of the game.
* @date	2018-11-2
*
* @param	user_name_addr	the first element address of the user name array.
* @param	game_times		the times user want to play.
* @param	name_length		the length of user name.
*/
user_info times_UI(user_info user)
{
	int i = 0;																	/* Declare i uses as run times of for loop. */
	char user_times[256] = { 0 };
	user.times.result = result_Error;

	while (user.times.result == result_Error)
	{
		system("cls");
		user.times.result = result_OK;							/* Pretend the times input is legal at first. */
		printf("Enter the times you want to play(must be a POSITIVE INTEGER and SMALLER THAN 50!): ");
		rewind(stdin);
		gets(user_times);
		rewind(stdin);

		if (strlen(user_times) != 0)
		{
			for (i = 0; i < strlen(user_times); i++)		/* Check if the times input is number. */
			{
				if (!isdigit(user_times[i]))
				{
					user.times.result = result_Error;			/* The times is not number, input illegal. */
					break;
				}
			}
		}
		if (user.times.result == result_OK)						/* Check if the times is less than 50. */
		{
			user.times.target = atoi(user_times);
			if (user.times.target <= 0 || user.times.target >= 50) user.times.result = result_Error;		/* The times is not less than 50, input illegal. */
		}
		if (user.times.result == result_Error)
		{
			printf("\nThe times you input is illegal, please try again!\n");
			Sleep(1500);
		}
	}

	return user;
}

/**
* @name	computer_select_get
* @brief	generate and print computer select result.
* @date	2018-11-2
*
* @return	the character computer choose: rock, scissors or paper.
*/
General_Select computer_select_get(void)
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

/**
* @name	user_select_get
* @brief	choose and print user select result.
* @date	2018-11-3
*
* @return	the character user choose: rock, scissors or paper.
*/
General_Select user_select_get(void)
{
	char select_input[256] = { 0 };							/* Declare select_input to store the character user select in string. */
	int i = 0;												/* Declare i uses as run times of for loop. */
	General_Result select_result = result_Error;			/* Declare to store the select_input result. */
	General_Select user_select;								/* Declare user_select to store the character user select. */

	printf("Please use a letter to choose what you want, the meanings are as follows:\n");
	printf("r: Rock \t\t s: Scissors \t\t p: Paper\n\n");

	while (select_result == result_Error)
	{
		printf_position("Computer choice is:", 0, 13);
		printf_delta("Your choice is(r/s/p): ", 30, 0);
		rewind(stdin);
		gets(select_input);
		rewind(stdin);

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

/**
* @name	rounds_UI
* @brief	rounds UI of the game.
* @date	2018-11-3
*
* @param	user_name_addr	the first element address of the user name array.
* @param	game_times		the times user want to play.
* @param	name_length		the length of user name.
* @return	the final winner of game: computer, user or none.
*/
user_info rounds_UI(user_info user)
{
	int remain_games, i;		/* Declare variables:
																			remain_games: store the remain times of game.
																			computer_win: store the times of computer win.
																			user_win: store the times of user win.*/
	General_Select computer_select, user_select;			/* Declare to store the character computer and user select. */
	Game_Player current_winner;				/* Declare to store the current and final winner of game. */

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

		user_select = user_select_get();					/* Call user_select_get function and store user select to the variable. */
		computer_select = computer_select_get();			/* Call computer_select_get function and store computer select to the variable. */
		current_winner = compare(computer_select, user_select);		/* Call compare function and store current winner to the variable. */
		if (current_winner == computer)
		{
			printf_position("Computer wins this time!\n", rounds_X_current_result_pos, rounds_Y_current_result_pos);
			user.times.lose++;									/* Add times of computer win. */
		}
		else if (current_winner == player)
		{
			printf_position("You win this time!\n", rounds_X_current_result_pos, rounds_Y_current_result_pos);
			user.times.win++;										/* Add times of user win. */
		}
		else
		{
			printf_position("Nobody wins this time!\n", rounds_X_current_result_pos, rounds_Y_current_result_pos);
			user.times.draw++;
		}

		printf("\nPress \"Enter\" to continue game...");
		rewind(stdin);
		while (getchar() != '\n');							/* Get "Enter" to continue game. */
		rewind(stdin);
	}

	/* Compare the times of computer and user win and judge and final winner. */
	if (user.times.lose > user.times.win) user.final_win = 0;
	else if (user.times.lose < user.times.win) user.final_win = 1;
	else user.final_win = 2;

	return user;									/* Return the final winner of the game. */
}

/**
* @name	compare
* @brief	compare the character that computer and user select.
* @date	2018-11-3
*
* @param	computer_select		the character computer select.
* @param	user_select			the character user select.
* @return	current winner: computer, user or none.
*/
Game_Player compare(General_Select computer_select, General_Select user_select)
{
	Game_Player current_winner;								/* Declare current_winner to store current winner. */

	if (computer_select == user_select) current_winner = none;		/* If the characters are same. Nobody win this time. */
	else
	{
		switch (computer_select)							/* Logic to judge the winner. */
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

	return current_winner;									/* Return the current winner. */
}

/**
* @name	final_UI
* @brief	final UI of the game.
* @date	2018-11-3
*
* @param	final_winner	the final winner.
* @param	option_choice	the user choice that whether to play the game again or not.
*/
void final_UI(user_info user)
{
	General_Result user_result = result_Error;				/* Declare user_result to store the user input result. */
	char user_choice[256] = { 0 };							/* Declare user_choice to store user choice as string. */
	int i = 0;												/* Declare i uses as run times of for loop. */

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

	printf_delta("Press \"Enter\" to back to menu...", 34, 1);
	while (getchar() != '\n');
	datasave(user);
	return user_choice[0];		/* Change the value on address option_choice to user_choice[0](to use in main function). */
}

void datasave(user_info user)
{
	int i = 0, j = 0;
	char temp = 0;
	user.file.pointer = fopen(user.file.name, "r+");
	fseek(user.file.pointer, strlen(user.name.detail) + strlen(user.passwd.detail) + 3, SEEK_SET);
	fscanf(user.file.pointer, "%d", &user.times.games);
	user.times.games++;
	fseek(user.file.pointer, -1, SEEK_CUR);				/* 遗留次数超过10次的bug */
	fprintf(user.file.pointer, "%d", user.times.games);
	user.record = (int*)malloc(user.times.games * sizeof(int));
	for (i = 0; i < user.times.games; i++)
		user.record[i] = (int*)malloc(6 * sizeof(int));

	fseek(user.file.pointer, 2, SEEK_CUR);
	for (i = 0; i < user.times.games - 1; i++)
	{
		for (j = 0; j < 6; j++)
			fscanf(user.file.pointer, "%d", &user.record[i][j]);
		while (1)
		{
			fscanf(user.file.pointer, "%c", &temp);
			if (temp == '\n') break;
		}
	}
	user.record[user.times.games - 1][0] = user.times.games;
	user.record[user.times.games - 1][1] = user.times.target;
	user.record[user.times.games - 1][2] = user.times.win;
	user.record[user.times.games - 1][3] = user.times.lose;
	user.record[user.times.games - 1][4] = user.times.draw;
	user.record[user.times.games - 1][5] = user.final_win;

	for (j = 0; j < 6; j++)								/* 遗留不显示第二次记录的bug */
	{
		if (j < 5)
			fprintf(user.file.pointer, "%d\t", user.record[user.times.games - 1][j]);
		else
			fprintf(user.file.pointer, "%d\n", user.record[user.times.games - 1][j]);
	}
	rewind(user.file.pointer);
	fclose(user.file.pointer);
	for (i = 0; i < user.times.games; i++)
		free(user.record[i]);
	free(user.record);
}

void review_UI(user_info user)
{
	int i, j;
	char temp = 0;

	system("cls");
	printf_position("Game History\n", 41, 1);
	user.file.pointer = fopen(user.file.name, "r+");
	fseek(user.file.pointer, strlen(user.name.detail) + strlen(user.passwd.detail) + 3, SEEK_SET);
	fscanf(user.file.pointer, "%d", &user.times.games);

	user.record = (int*)malloc(user.times.games * sizeof(int));
	for (i = 0; i < user.times.games; i++)
		user.record[i] = (int*)malloc(6 * sizeof(int));
	
	fseek(user.file.pointer, 2, SEEK_CUR);
	for (i = 0; i < user.times.games; i++)
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

	printf_position("Games\tTotal\tWin\tLose\tDraw\tWinner\n", 24, 3);
	printf_delta("", 24, 0);
	for (i = 0; i < 46; i++)
		printf("-");
	printf("\n");
	for (i = 0; i < user.times.games - 1; i++)			/* 遗留第二次无法记录的bug */
	{
		printf_delta("", 24, 0);
		for (j = 0; j < 6; j++)
			printf(" %d\t", user.record[i][j]);
		printf("\n");
	}
	for (i = 0; i < user.times.games; i++)
		free(user.record[i]);
	free(user.record);

	printf_delta("Press \"Enter\" to back to menu...", 34, 2);
	while (getchar() != '\n');
}

void clear_UI(user_info user)
{
	char user_choice[256] = { 0 };							/* Declare user_choice array to store user choice string. */
	int i = 0;												/* Declare i uses as run times of for loop. */
	General_Result user_result = result_Error;				/* Declare user_result to store the option input result. */

	system("cls");
	printf_position("Would you really like to clear game history(y/n)?\n", 25, 0);
	printf_delta("WARNING: This operation is irrevocable!\n", 30, 1);

	while (user_result == result_Error)						/* When user input is illegal. */
	{
		printf_position("Your choice is: ", 42, 4);
		rewind(stdin);
		gets(user_choice);
		rewind(stdin);

		if (strlen(user_choice) == 1)						/* If the input is 1 character. */
		{
			switch (user_choice[0])							/* Judge the user choice. */
			{
			case 'y':
			case 'n':
				user_result = result_OK;					/* The choice is 'a' or 'b', input is legal. */
				break;
			default:
				user_result = result_Error;					/* The choice is others, input is illegal. */
			}
		}
		if (user_result == result_Error)					/* If the input is illegal. */
		{
			printf_delta("Your input is illegal, please try again!\n", 30, 1);
			Sleep(1500);									/* Wait for 1500 ms. */
			printf_delta("", 0, -3);						/* Change the position of cursor. */
			for (i = 0; i < X_LENGTH; i++)					/* Clear incorrect output on console. */
				printf(" ");
			printf("\n\n");
			for (i = 0; i < X_LENGTH; i++)
				printf(" ");
		}
	}

	if (user_choice[0] == 'y')								/* 遗留清除后第一次游戏无法添加记录的bug */
	{
		user.times.games = 0;
		user.file.pointer = fopen(user.file.name, "w+");
		fprintf(user.file.pointer, "%s,%s\n%d\n", user.name.detail, user.passwd.detail, user.times.games);
		rewind(user.file.pointer);
		fclose(user.file.pointer);

		printf_delta("Clear successfully!", 40, 1);
		Sleep(1500);
	}
}

/**
 * @name	printf_position
 * @brief	printf on specific position.
 * @date	2018-11-3
 *
 * @param	data	string needs to be print.
 * @param	init_X	x-axis coordinates of the first character.
 * @param	init_Y	y-axis coordinates of the first character.
 */
void printf_position(char *data, int init_X, int init_Y)
{
	HANDLE hd;												/* Declare hd to store handle to standard output. */
	COORD position;											/* Declare position to store coordinates. */

	hd = GetStdHandle(STD_OUTPUT_HANDLE);					/* Get handle to standard output. */
	position.X = init_X;									/* Set the x-axis coordinates. */
	position.Y = init_Y;									/* Set the y-axis coordinates. */

	SetConsoleCursorPosition(hd, position);					/* Move the cursor to the set position of console. */
	printf("%s", data);										/* Print the data. */
}

/**
 * @name	printf_delta
 * @brief	printf on incremental position.
 * @date	2018-11-3
 *
 * @param	data	string needs to be print.
 * @param	init_X	x-axis coordinates of the first character.
 * @param	init_Y	y-axis coordinates of the first character.
 */
void printf_delta(char *data, int delta_X, int delta_Y)
{
	HANDLE hd;												/* Declare hd to store handle to standard output. */
	COORD position;											/* Declare position to store coordinates. */
	CONSOLE_SCREEN_BUFFER_INFO console_buf;					/* Declare to store buffer info of console. */

	hd = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hd, &console_buf);			/* Get buffer info of console. */
	position.X = console_buf.dwCursorPosition.X;			/* Get current x-axis coordinates of cursor. */
	position.Y = console_buf.dwCursorPosition.Y;			/* Get current y-axis coordinates of cursor. */
	position.X += delta_X;
	position.Y += delta_Y;
	
	SetConsoleCursorPosition(hd, position);					/* Move the cursor to the set position of console. */
	printf("%s", data);										/* Print the data. */
}

/**
 * @name	print_rock
 * @brief	print the picture of rock.
 * @date	2018-11-3
 *
 * @param	size	mini or normal.
 * @param	bias_X	x-axis coordinates of the picture.
 * @param	bias_Y	y-axis coordinates of the picture.
 */
void print_rock(Character_Size size, int bias_X, int bias_Y)
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

/**
 * @name	print_scissors
 * @brief	print the picture of scissors.
 * @date	2018-11-3
 *
 * @param	size	mini or normal.
 * @param	bias_X	x-axis coordinates of the picture.
 * @param	bias_Y	y-axis coordinates of the picture.
 */
void print_scissors(Character_Size size, int bias_X, int bias_Y)
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

/**
 * @name	print_paper
 * @brief	print the picture of paper.
 * @date	2018-11-3
 *
 * @param	size	mini or normal.
 * @param	bias_X	x-axis coordinates of the picture.
 * @param	bias_Y	y-axis coordinates of the picture.
 */
void print_paper(Character_Size size, int bias_X, int bias_Y)
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