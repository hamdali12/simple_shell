#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * type_prompt - Displays a prompt for the user.
 */
void type_prompt(void) {
    printf("$ ");
}

/**
 * read_command - Reads a command from the user.
 * @command: Buffer to store the entered command.
 */
void read_command(char command[]) {
    fgets(command, MAX_COMMAND_LENGTH, stdin);

    // Remove the newline character at the end
    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }
}

/**
 * execute_command - Executes the specified command with arguments.
 * @arguments: Array of command-line arguments.
 */
void execute_command(char *arguments[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execvp(arguments[0], arguments);

        // If execvp fails
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: wait for the child to complete
        wait(NULL);
    }
}

/**
 * print_environment - Prints the current environment variables.
 */
void print_environment(void) {
    extern char **environ;
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

int main(void) {
    char command[MAX_COMMAND_LENGTH];
    char *arguments[20];

    while (1) {
        type_prompt();
        read_command(command);

        // Check for end of file (Ctrl+D)
        if (feof(stdin)) {
            printf("\n");
            break;
        }

        // Tokenize the command line into arguments
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            arguments[i++] = token;
            token = strtok(NULL, " ");
        }

        arguments[i] = NULL;

        // Check if the command is a built-in command
        if (strcmp(arguments[0], "exit") == 0) {
            printf("Exiting the shell\n");
            break;
        } else if (strcmp(arguments[0], "env") == 0) {
            // Built-in command: env
            print_environment();
        } else {
            // Execute the command
            execute_command(arguments);
        }
    }

    return 0;
}
