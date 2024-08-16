#include "headers.h"



void handle_output_redirection(char *filename, char *command_tokens[], int num_tokens ) {
    
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

           

        
            
            pid_t pid = fork();
            if (pid == 0) {
                
                dup2(fd, STDOUT_FILENO);
                close(fd);

                
                execvp(command_tokens[0], command_tokens);
                
                
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", command_tokens[0]);
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
               
                int status;
                waitpid(pid, &status, 0); 
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    printf("Command failed\n");
                }
            } else {
                perror("Fork failed");
            }
}




void handle_append_redirection(char *filename, char *command_tokens[], int num_tokens) {
            int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            
            pid_t pid = fork();
            if (pid == 0) {
                
                dup2(fd, STDOUT_FILENO);
                close(fd);

                
                execvp(command_tokens[0], command_tokens);
                
               
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", command_tokens[0]);
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                
                int status;
                waitpid(pid, &status, 0); 
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    printf("Command failed\n");
                }
            } else {
                perror("Fork failed");
            }
}



void handle_input_redirection(char *filename, char *command[], int num_tokens) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
   


                pid_t pid = fork();
                if (pid == 0) {

                     dup2(fd, STDIN_FILENO);
                     close(fd);
                 
                    execvp(command[0], command);
                 
                    perror("execvp");
                    exit(EXIT_FAILURE);
                } else if (pid > 0) {
          
                    wait(NULL); 
                } else {
                    perror("Fork failed");
                }
}




