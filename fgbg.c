#include "headers.h"



void bringToForeground(int pid) {
    int found = 0;
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].pid == pid) {
           
            if (kill(pid, SIGCONT) == 0) {
                
                processes[i].completed = 0;
                found = 1;
                
                waitpid(pid, NULL, 0);
            } else {
                perror("kill");
            }
            break;
        }
    }

    if (!found) {
        printf("No such process found with PID %d\n", pid);
    }
}


void resumeInBackground(int pid) {
    int found = 0;
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].pid == pid) {
            
            if (kill(pid, SIGCONT) == 0) {
                
                processes[i].completed = 0;
                found = 1;
                printf("[%d] %s - Running (in the background)\n", processes[i].pid, processes[i].command);
            } else {
                perror("kill");
            }
            break;
        }
    }

    if (!found) {
        printf("No such process found with PID %d\n", pid);
    }
}



void addProcess(int pid, const char* command) {
    if (num_processes < MAX_BACKGROUND_PROCESSES) {
        processes[num_processes].pid = pid;
        strncpy(processes[num_processes].command, command, sizeof(processes[num_processes].command));
        processes[num_processes].completed = 0; 
        num_processes++;
    }
}





void markProcessAsCompleted(int pid) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].pid == pid) {
            processes[i].completed = 1; 
            break;
        }
    }
}