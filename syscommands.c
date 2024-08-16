#include "headers.h"

struct BackgroundProcess background_processes[MAX_BACKGROUND_PROCESSES];
int num_background_processes = 0;

void print_background_messages() {
    
    for (int i = 0; i < num_background_processes; i++) {
        int status;
        int result = waitpid(background_processes[i].pid, &status, WNOHANG);

        if (result > 0) {
            background_processes[i].end_time = clock(); 
            if (WIFEXITED(status)) {
                printf("Background process '%s' (PID %d) exited normally\n",
                       background_processes[i].command, background_processes[i].pid);
                background_processes[i].completed = 1;
            } else {
                printf("Background process '%s' (PID %d) exited abnormally\n",
                       background_processes[i].command, background_processes[i].pid);
                background_processes[i].completed = 2;
            }
        }
    }

    int new_num_background_processes = 0;
    for (int i = 0; i < num_background_processes; i++) {
        if (background_processes[i].completed == 0 && background_processes[i].completed == 2) {
            background_processes[new_num_background_processes] = background_processes[i];
            new_num_background_processes++;
        }
    }
    num_background_processes = new_num_background_processes;
}

