#include "headers.h"

void sendSignalToProcess(int pid, int signalNumber) {
    
    int result = kill(pid, signalNumber);
    
    if (result == -1) {
        perror("kill"); 
    }
    
    
}





// void handleCtrlC(int signum) {
//     if (foreground_pid > 0) {
//         sendSignalToProcess(foreground_pid, SIGINT);
//     }
// }


void handleCtrlZ(int signum) {
    
    printf("\nCtrl-Z received. Process stopped.\n");
    if (foreground_pid > 0) {
        sendSignalToProcess(foreground_pid, SIGTSTP);
    }
}

void handleCtrlD() {
   
    printf("Ctrl-D received. Logging out of the shell.\n");
    
   
    for (int i = 0; i < num_background_processes; i++) {
        sendSignalToProcess(background_processes[i].pid, SIGKILL);
    }
    
    
    exit(0);
}


int isValidSignalNumber(const char *signalStr) {
    while (*signalStr) {
        if (!isdigit(*signalStr)) {
            return 0; 
        }
        signalStr++;
    }

    int signalNumber = atoi(signalStr);
    return (signalNumber >= 1 && signalNumber <= 32);
}




void handlePingCommand(char *arg1, char *arg2) {
    int pid = atoi(arg1);
    if (pid <= 0) {
        printf("Invalid PID: %s\n", arg1);
        return;
    }

    if (isValidSignalNumber(arg2)) {
        printf("Invalid signal number: %s\n", arg2);
        return;
    }

    int signalNumber = atoi(arg2) % 32;

    if (signalNumber == 19) {
        
        sendSignalToProcess(pid, SIGSTOP);
        updateProcessStatus(pid, signalNumber);
    } else if (signalNumber == 9) {
        
        sendSignalToProcess(pid, SIGKILL);
        updateProcessStatus(pid, signalNumber);
    } else if (signalNumber == 18) {
        
        sendSignalToProcess(pid, SIGCONT);
       
        updateProcessStatus(pid, signalNumber);
    } else {
        
        printf("Unsupported signal number: %d\n", signalNumber);
    }
}



void updateProcessStatus(int pid, int signalNumber) {
    int status;

    
    int index = -1;
    for (int i = 0; i < num_background_processes; i++) {
        if (background_processes[i].pid == pid) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        
        if (signalNumber == 19) {
            
            background_processes[index].completed = 0; 
            background_processes[index].end_time = clock(); 
            printf("Process with PID %d was stopped (SIGSTOP)\n", pid);
        } else if (signalNumber == 18) {
            
            background_processes[index].completed = 1; 
            background_processes[index].end_time = clock(); 
            printf("Process with PID %d was resumed (SIGCONT)\n", pid);
        } else if (WIFEXITED(status)) {
           
            background_processes[index].completed = 1; 
            background_processes[index].exit_status = WEXITSTATUS(status);
            background_processes[index].end_time = clock(); 
            printf("Process with PID %d exited with status %d\n", pid, WEXITSTATUS(status));
            for (int j = index; j < num_background_processes - 1; j++) {
                background_processes[j] = background_processes[j + 1];
            }
            num_background_processes--;
        } else if (WIFSIGNALED(status)) {
            
            background_processes[index].completed = 1; 
            background_processes[index].exit_status = -1; 
            background_processes[index].end_time = clock(); 
            printf("Process with PID %d terminated by signal %d\n", pid, WTERMSIG(status));

            for (int j = index; j < num_background_processes - 1; j++) {
                background_processes[j] = background_processes[j + 1];
            }
            num_background_processes--;
        }
         
    } else {
        
        printf("No such process found with PID %d\n", pid);
    }
}
