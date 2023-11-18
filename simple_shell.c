#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PATH_LEN 256

/**
 * type_prompt - Displays a prompt for the user.
 */
void type_prompt(void)
{
	printf("$ ");
}

/**
 * read_command - Reads a command from the user.
 * @command: Buffer to store the entered command.
 */
void read_command(char command[])
{
	fgets(command, 100, stdin);

	size_t len = strlen(command);
	if (len > 0 && command[len - 1] == '\n')
	{
		command[len - 1] = '\0';
	}
}

/**
 * is_command_exist - Checks if the command exists in the PATH.
 * @command: The command to check.
 * @path: The PATH environment variable.
 * Return: 1 if the command exists, 0 otherwise.
 */
int is_command_exist(const char *command, const char *path)
{
	char *path_copy = strdup(path);
	char *token = strtok(path_copy, ":");

	while (token != NULL)
	{
		char full_path[MAX_PATH_LEN];
		snprintf(full_path, MAX_PATH_LEN, "%s/%s", token, command);

		if (access(full_path, F_OK) == 0)
		{
			free(path_copy);
			return (1);
		}

		token = strtok(NULL, ":");
	}

	free(path_copy);
	return (0);
}

/**
 * execute_command - Executes the specified command with arguments.
 * @arguments: Array of command-line arguments.
 * @path: The PATH environment variable.
 */
void execute_command(char *arguments[], const char *path)
{
	if (!is_command_exist(arguments[0], path))
	{
		printf("Command not found: %s\n", arguments[0]);
		return;
	}

	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execvp(arguments[0], arguments);

		perror("exec");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
	}
}

/**
 * print_environment - Prints the current environment variables.
 */
void print_environment(void)
{
	extern char **environ;
	for (char **env = environ; *env != NULL; env++)
	{
		printf("%s\n", *env);
	}
}

/**
 * main - Entry point of the simple shell.
 * Return: 0 on success.
 */
int main(void)
{
	char command[100];
	char *arguments[20];
	char *path = getenv("PATH");

	while (1)
	{
		type_prompt();
		read_command(command);

		if (feof(stdin))
		{
			printf("\n");
			break;
		}

		char *token = strtok(command, " ");
		int i = 0;

		while (token != NULL)
		{
			arguments[i++] = token;
			token = strtok(NULL, " ");
		}

		arguments[i] = NULL;

		if (strcmp(arguments[0], "exit") == 0)
		{
			printf("Exiting the shell\n");
			break;
		}
		else if (strcmp(arguments[0], "env") == 0)
		{
			print_environment();
		}
		else
			execute_command(arguments, path);
		}
	}

	return (0);
}
