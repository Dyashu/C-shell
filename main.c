#include "headers.h"


struct PastEvent* past_events_head = NULL;


// In main.c fgbg
struct ProcessInfo processes[MAX_BACKGROUND_PROCESSES];
int num_processes = 0; 
int foreground_pid = -1;



void executeWarp(const char* warp_path, char* current_dir);

void executePeek(const char* path, int a_flag, int l_flag);



int main() {
    
    char current_dir[MAX_PATH_LEN];
if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
    perror("getcwd");
    exit(1);
}


// signal(SIGINT, handleCtrlC);
signal(SIGTSTP, handleCtrlZ);
signal(SIGKILL, handleCtrlD);


 char input[4096];
//  char current_command[MAX_EVENT_LENGTH];
    


    while (1) {
            time_t start;
            // printf("%ld\n",start);
        
    prompt( input, start) ;
        fgets(input, 4096, stdin);






 if (strncmp(input, "fg", 2) == 0) {
            int pid;
            if (sscanf(input, "fg %d", &pid) == 1) {
                bringToForeground(pid);
                foreground_pid = pid;
                continue;
            }
}




if (strncmp(input, "bg", 2) == 0) {
            int pid;
            if (sscanf(input, "bg %d", &pid) == 1) {
                resumeInBackground(pid);
                if (foreground_pid == pid) {
                    foreground_pid = -1; 
                }
                continue;
            }
}



if (strcmp(input, "activities\n") == 0) {
            executeActivitiesCommand();
            continue;
}







// Check of completed background processes
for (int i = 0; i < num_background_processes; i++) {
    int status;
    int result = waitpid(background_processes[i].pid, &status, WNOHANG);
    if (result == -1) {
        perror("waitpid");
    } else if (result > 0) {
        
        background_processes[i].completed = 1;
        background_processes[i].end_time = clock();
        background_processes[i].exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

        printf("[%d] %s %s\n", background_processes[i].pid,
               background_processes[i].command,
               WIFEXITED(status) ? "exited normally" : "exited abnormally");

       
        for (int j = i; j < num_background_processes - 1; j++) {
            background_processes[j] = background_processes[j + 1];
        }
        num_background_processes--;
        markProcessAsCompleted(background_processes[i].pid);
    }
    
}








        size_t len = strcspn(input, "\n");
        if (len < strlen(input)) {
            input[len] = '\0';
        }





// pastevents command
if (strcmp(input, "pastevents") == 0) {
    
    printPastEvents(past_events_head);
    continue; 
}else if (strcmp(input, "pastevents purge") == 0) {
    
    clearPastEvents(&past_events_head);
    continue; 
}else if (strncmp(input, "pastevents execute", 18) == 0) {
    int index;
    if (sscanf(input, "pastevents execute %d", &index) == 1) {
        executePastEvents(index, &past_events_head);
        continue;
    }
}else if(strcmp(input, "pastevents") != 0 && strcmp(input, "pastevents purge") != 0 && strncmp(input, "pastevents execute", 18) != 0){
   
    // printf("Storing command: %s\n", commands[i]);
    addPastEvent(&past_events_head, input);
} 


// Handle command
if (strstr(input, "|") != NULL) {
            
            handle_pipes(input);

}



        clock_t start_time = clock();
        
        char* command;
        char* commands[MAX_COMMANDS];
        int num_commands = 0;

        command = strtok(input, ";\n");
        while (command != NULL) {
            commands[num_commands] = command;
            num_commands++;
            command = strtok(NULL, ";\n");
        }

        

        
        
        for (int i = 0; i < num_commands; i++) {

        
            char* token;
                char* command_tokens[MAX_TOKENS];
                int num_tokens = 0;
                int run_in_background = 0;

                token = strtok(commands[i], " \t\n");
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
            //  printf("%s\n",command_tokens[0]);


for (int i = 0; i < num_tokens; i++) {
    int len = strlen(command_tokens[i]);
    if (len > 1 && command_tokens[i][0] == '"' && command_tokens[i][len - 1] == '"') {
        
        memmove(command_tokens[i], command_tokens[i] + 1, len - 2);
        command_tokens[i][len - 2] = '\0';
    }
}

if(strcmp(command_tokens[0], "iMan") == 0){
    
     if ( num_tokens== 2){
                  
        fetchAndDisplayManPage(command_tokens[1]);
        // printf("A76487534\n");
    }else{
        printf("Usage: iMan <command_name>\n");
    }
    continue;
}else




if (strcmp(command_tokens[0], "warp") == 0) {

                if (num_tokens > 1) {
                    executeWarp(command_tokens[1], current_dir);
                } else {
                    executeWarp("", current_dir);
                    // printf("a\n");
                }
                continue;
}else 





if (strcmp(command_tokens[0], "peek") == 0) {
                //  printf("a\n");
            int a_flag = 0;
            int l_flag = 0;
            const char *path = current_dir; 

          
            for (int i = 1; i < num_tokens; i++) {
                if (command_tokens[i][0] == '-') {
                    if (strcmp(command_tokens[i], "-a") == 0) {
                        a_flag = 1;
                    }
                    if (strcmp(command_tokens[i], "-l") == 0) {
                        l_flag = 1;
                    }
                    if (strcmp(command_tokens[i], "-al") == 0) {
                        l_flag = 1;
                        a_flag= 1;
                    }
                    if (strcmp(command_tokens[i], "-la") == 0) {
                        l_flag = 1;
                        a_flag= 1;
                    }
                    if (strcmp(command_tokens[i], "-l-a") == 0) {
                        l_flag = 1;
                        a_flag= 1;
                    }
                    if (strcmp(command_tokens[i], "-a-l") == 0) {
                        l_flag = 1;
                        a_flag= 1;
                    }
                } 
                else {
                    path = command_tokens[i];
                }
            }
            
            executePeek(path, a_flag, l_flag);
            continue;
}else



 if (strcmp(command_tokens[0], "proclore") == 0) {
            int target_pid = getpid(); 

            if (command_tokens[1] != NULL) {
                char *pid_str = command_tokens[1];
                target_pid = atoi(pid_str);

                if (target_pid <= 0) {
                    printf("Invalid PID: %s\n", pid_str);
                    continue;
                }
            }

            proclore(target_pid);
            continue;
}else










if (strcmp(command_tokens[num_tokens - 2], "<") == 0) {
                
                if (num_tokens < 3) {
                    printf("Invalid input redirection format\n");
                    continue; 
                }

                char *filename = command_tokens[num_tokens - 1];
                command_tokens[num_tokens - 2] = NULL; 
                
                char *command[MAX_TOKENS];
                for (int i = 0; i < num_tokens - 2; i++) {
                    command[i] = command_tokens[i];
                }
                command[num_tokens - 2] = NULL;

                
                handle_input_redirection(filename, command, num_tokens-2);
                continue;
}else






if (strcmp(command_tokens[num_tokens - 2], ">") == 0) {
           
            char *filename = command_tokens[num_tokens - 1];

                   
                    char *command_before_redirection[MAX_TOKENS];
                    int num_tokens_before_redirection = 0;

                    
                    for (int i = 0; i < num_tokens; i++) {
                        if (strcmp(command_tokens[i], ">") == 0) {
                            
                            break;
                        }
                        command_before_redirection[num_tokens_before_redirection] = command_tokens[i];
                        num_tokens_before_redirection++;
                        // printf("%s\n",command_tokens[i]);
                    }

                    
                    command_before_redirection[num_tokens_before_redirection] = NULL;

                    handle_output_redirection(filename,  command_before_redirection, num_tokens_before_redirection );
                    continue;

}else






if (strcmp(command_tokens[num_tokens - 2], ">>") == 0) {
                    char *filename = command_tokens[num_tokens - 1];
                    
                    
                    char *command_before_redirection[MAX_TOKENS];
                    int num_tokens_before_redirection = 0;

                    
                    for (int i = 0; i < num_tokens - 2; i++) {
                        command_before_redirection[num_tokens_before_redirection] = command_tokens[i];
                        num_tokens_before_redirection++;
                    }

                    
                    command_before_redirection[num_tokens_before_redirection] = NULL;

                    handle_append_redirection(filename, command_before_redirection, num_tokens_before_redirection);
                    continue;
}else




if (strcmp(command_tokens[0], "ping") == 0) {
    
    if (command_tokens[1] != NULL && command_tokens[2] != NULL) {
        handlePingCommand(command_tokens[1], command_tokens[2]);
    } else {
        printf("Usage: ping <pid> <signal_number>\n");
    }
    continue; 
}
else

if (strcmp(command_tokens[0], "neonate") == 0) {
        if (num_tokens != 3 || strcmp(command_tokens[1], "-n") != 0) {
            fprintf(stderr, "Usage: neonate -n [time_arg]\n");
            return 1;
        }

        int time_arg = atoi(command_tokens[2]);
        if (time_arg <= 0) {
            fprintf(stderr, "Invalid time_arg: %s\n", command_tokens[2]);
            return 1;
        }

        enableRawMode();
        printf("Press 'x' to stop.\n");

        printMostRecentPID();

        disableRawMode();
        
        continue;
    }else


if (strncmp(input, "seek", 4) == 0) {
    int isDirectoryFlag = 0;
    int isFileFlag = 0;
    int exactMatchFlag = 0;
    char *search = NULL;
    char *targetDir = NULL;

    for (int i = 1; i < num_tokens; i++) {
        if (strcmp(command_tokens[i], "-d") == 0) {
            isDirectoryFlag = 1;
        } else if (strcmp(command_tokens[i], "-f") == 0) {
            isFileFlag = 1;
        } else if (strcmp(command_tokens[i], "-e") == 0) {
            exactMatchFlag = 1;
        } else {
            if (search == NULL) {
                search = command_tokens[i];
            } else if (targetDir == NULL) {
                targetDir = command_tokens[i];
            }
        }
    }

    
    if ((isDirectoryFlag && isFileFlag) || (exactMatchFlag && (isDirectoryFlag || isFileFlag))) {
        fprintf(stderr, "Invalid flags!\n");
        return 1;
    }

    
    if (targetDir == NULL) {
        targetDir = ".";
    }

    seekInDirectory(targetDir, search, isDirectoryFlag, isFileFlag, exactMatchFlag);

    continue;
}
else




if (run_in_background) {
    int pid = fork();
    if (pid == 0) {
       
        execvp(command_tokens[0], command_tokens);
        
        fprintf(stderr, "ERROR: '%s' is not a valid command\n", command_tokens[0]);
        exit(1);
    } else if (pid > 0) {
        
        if (num_background_processes < MAX_BACKGROUND_PROCESSES) {
            strcpy(background_processes[num_background_processes].command, command_tokens[0]);
            background_processes[num_background_processes].pid = pid;
            background_processes[num_background_processes].completed = 0;
            background_processes[num_background_processes].start_time = clock();
            num_background_processes++;

            
            printf("[%d]\n", pid);
            

            addProcess(pid, command_tokens[0]);
        }
    } else {
        perror("Fork failed");
    }

    
   
} else {
   
    if (strcmp(command_tokens[0], "sleep") == 0) {
        if (num_tokens > 1) {
            int sleep_duration = atoi(command_tokens[1]);
            time_t start_time;
            time(&start_time);
        
            int pid = fork();
            if (pid == 0) {
                
                execlp("sleep", "sleep", command_tokens[1], NULL);
                
                fprintf(stderr, "ERROR: 'sleep' command not found\n");
                exit(1);
            } else if (pid > 0) {
                
                waitpid(pid, NULL, 0); 

                time_t end_time;
                time(&end_time);
                double elapsed_time = difftime(end_time, start_time);
                start = elapsed_time;
            } else {
                perror("Fork failed");
            }
        } else {
            fprintf(stderr, "ERROR: 'sleep' command requires a duration argument\n");
        }
        continue;
    }

}







if (run_in_background == 0){

        time_t start_time;
        time(&start_time);



int pid = fork();
if (pid == 0) {
    
    execvp(command_tokens[0], command_tokens);
    
    
    fprintf(stderr, "ERROR: '%s' is not a valid command\n", command_tokens[0]);
    exit(1);
} else if (pid > 0) {
   
    if (num_commands > 0 && strcmp(command_tokens[num_commands - 1], "&") != 0) {
        wait(NULL);
                time_t end_time;
                time(&end_time);
                double elapsed_time = difftime(end_time, start_time);
                start = elapsed_time;
    }
} else {
    perror("Fork failed");
}
}



        }


        }
     
    
    return 0;
}
