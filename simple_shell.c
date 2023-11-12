#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"

/**
 * read_command - Reads a command from the standard input and parses it into
 *                a command and its parameters.
 *
 * @cmd: A character array to store the command.
 * @par: An array of character pointers to store the parameters.
 *
 * Description:
 * This function reads a command line from the standard input and parses it
 * into a command and its parameters. The command is stored in the provided
 * `cmd` array, and the parameters are stored in the `par` array.
 *
 * Parameters:
 *   - cmd: A character array with sufficient space to store the command.
 *   - par: An array of character pointers to store the parameters.
 *
 * Return:
 * This function does not return a value.
 *
 * Usage:
 *   char command[100];
 *   char *parameters[10];
 *   read_command(command, parameters);
 */
void read_command(char cmd[], char *par[])
{
	char line[1024];
	int count = 0, i = 0, j;
	char *array[100], *pch;

	for (;;)
	{
		int c = fgetc(stdin);
		line[count++] = (char) c;
		if (c == '\n')
		{
			break;
		}
        if (count == 1)
		{
			return;
		}
		pch = strtok(line, "\n");

		while (pch != NULL)
		{
			array[i++] = strdup(pch);
			pch = strtok(NULL, "\n");
		}

		strcpy(cmd, array[0]);

		for (j = 0; j < i; j++)
		{
			par[j] = array[j];
			par[i] = NULL;
		}
	}
}

/**
 * type_prompt - Displays a command prompt on the standard output.
 *
 * Description:
 * This function displays a command prompt on the standard output, typically
 * represented by a '#' symbol. It also includes a feature to clear the screen
 * on the first invocation to provide a clean starting environment.
 *
 * Usage:
 *   type_prompt();
 *
 * Example:
 *   After calling `type_prompt()`, the output might be:
 *   #
 *
 * Note:
 *   The function includes a feature to clear the screen on the first invocation,
 *   enhancing the user interface by providing a clean starting environment.
 */
void type_prompt()
{
	static int first_time = 1;
	if (first_time)
	{
		const char* CLEAR_SCREEN_ANSI = "\033[1;1H\033[2J";
		write(STDERR_FILENO, CLEAR_SCREEN_ANSI, 12);
		first_time = 0;
	}

	printf("#");
}

/**
 * main - Entry point of the custom shell program.
 *
 * Description:
 * This program emulates a simple shell. It continuously prompts the user for
 * commands, reads and parses the input, and executes the specified command.
 * The shell supports a basic set of commands and includes a feature to exit
 * when the user enters the "exit" command.
 * Note:
 *   The shell uses fork() and execvp() to execute commands in a child process.
 *   It also includes a basic implementation of a command prompt and input
 *   processing. The program will keep running until the user enters "exit".
 */
int main() 
{
	char cmd[100], command[100], *parameters[20];

	char *envp[] = { (char *) "PATH=C:\\Windows\\System32", 0 };
	while(1)
	{
		type_prompt();
		read_command(command, parameters);
		if (fork() != 0)
		{
			wait(NULL);
        }
		else
		{
			strcpy(cmd, "C:\\Windows\\System32");
			strcpy(cmd, command);
			execvp(cmd, parameters, envp);
		}
		if (strcmp(command, "exit") == 0)
		{
			break;
		}
	}
	return 0;
}
