#include "headers.h"



void executeCommand(char *command) {
    char *command_tokens[MAX_TOKENS];
    int num_tokens = 0;
    int run_in_background = 0;

    
    char *token = strtok(command, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            run_in_background = 1;
        } else {
            command_tokens[num_tokens] = token;
            num_tokens++;
        }
        token = strtok(NULL, " \t\n");
    }
    command_tokens[num_tokens] = NULL;

    
    char *input_file = NULL;
    char *output_file = NULL;
    int input_fd, output_fd;

    for (int i = 0; i < num_tokens; i++) {
        if (strcmp(command_tokens[i], "<") == 0) {
            if (i < num_tokens - 1) {
                input_file = command_tokens[i + 1];
                i++;
            }
        } else if (strcmp(command_tokens[i], ">") == 0) {
            if (i < num_tokens - 1) {
                output_file = command_tokens[i + 1];
                i++;
            }
        } else if (strcmp(command_tokens[i], ">>") == 0) {
            if (i < num_tokens - 1) {
                output_file = command_tokens[i + 1];
                i++;
            }
        }
    }

    
    if (input_file != NULL) {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
        
    }

   
    if (output_file != NULL) {
        if (strstr(command, ">>") != NULL) {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (output_fd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
        
    }

   
    execvp(command_tokens[0], command_tokens);
    perror("execvp");
    exit(EXIT_FAILURE);
}



void handle_pipes(char *input) {
    char *pipe_commands[MAX_COMMANDS];
    int num_pipe_commands = 0;

    
    char *token = strtok(input, "|");
    while (token != NULL) {
        pipe_commands[num_pipe_commands] = token;
        num_pipe_commands++;
        token = strtok(NULL, "|");
    }

    int input_fd = STDIN_FILENO;

    for (int i = 0; i < num_pipe_commands; i++) {
        int pipefd[2];
        pipe(pipefd);
        pid_t pid = fork();

        if (pid == 0) { 
            dup2(input_fd, STDIN_FILENO);  
            if (i < num_pipe_commands - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
            }

            
            close(pipefd[0]);
            close(pipefd[1]);

            
            executeCommand(pipe_commands[i]);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else { 
            
            close(pipefd[1]);
            input_fd = pipefd[0]; 
            wait(NULL);
        }
    }
}