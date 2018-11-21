/*************************************************************************************
 * Copyright (C) 2018
 * @File name: 1718112_3.c
 * @Author: Ziqi Yang
 * @Version: 0.0.0
 * @Date: 2018-11-21
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
	name_info		name;
	passwd_info		passwd;
	int				record[1][5];
	file_info		file;
} user_info;

/* Function declaration Start. */
void welcome_UI(char *option_choice);
void login_UI(void);
void signup_UI(void);
void printf_position(char *data, int init_X, int init_Y);
void printf_delta(char *data, int delta_X, int delta_Y);
void print_rock(Character_Size size, int bias_X, int bias_Y);
void print_scissors(Character_Size size, int bias_X, int bias_Y);
void print_paper(Character_Size size, int bias_X, int bias_Y);
/* Function declaration End. */

int main(void)
{
	char *pOption_choice, option_choice;

	pOption_choice = &option_choice;

	system("mode con cols=100 lines=34");					/* Set the width of console is 100 and height is 34. */
	system("color 1f");										/* Set the color of console background in blue and font in white. */
	do							/* Call welcome_UI function. */
	{
		welcome_UI(pOption_choice);
		if (option_choice == 'a') login_UI();
		else if(option_choice == 'b') signup_UI();
	} while (option_choice != 'c');

	return 0;												/* Programm run successfully. */
}

/**
 * @name	welcome_UI
 * @brief	welcome UI of the game.
 * @date	2018-11-2
 *
 * @param	option_choice		the option user choose, which to decide start or exit game.
 */
void welcome_UI(char *option_choice)
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
		printf_position("Your choice is(a/b): ",34,17);
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
			printf_delta("Your input is illegal, please try again!\n", 26, 1);
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

	*option_choice = user_choice[0];			/* Change the value on address option_choice to user_choice[0](to use in main function). */
}

void login_UI(void)
{
	int i = 0, j = 0;
	char target_user_passwd[256] = { 0 };
	user_info user;

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
		strcat(user.file.name, "_info.txt");
		user.file.pointer = fopen(user.file.name, "r");
		if (user.file.pointer == NULL)
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
}

void signup_UI(void)
{
	int i = 0, j = 0;
	user_info user;

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
			strcat(user.file.name, "_info.txt");
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
	fprintf(user.file.pointer, "%s,%s", user.name.detail, user.passwd.detail);
	rewind(user.file.pointer);
	fclose(user.file.pointer);
	printf("\nAccount created successfully!");
	Sleep(1000);
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