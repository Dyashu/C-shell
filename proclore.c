#include "headers.h"
#include <errno.h>



const char *getStatusString(int status) {
    if (WIFEXITED(status) || WIFSIGNALED(status) || WIFCONTINUED(status)) {
        return "R";
    } else if (WIFSTOPPED(status)) {
        return "S";
    } else {
        return "Z";
    }
}



char *getExecutablePath(int pid) {
    char path[1024];
    sprintf(path, "/proc/%d/exe", pid);

 
    char buffer[1024];
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);

    if (len != -1) {
        buffer[len] = '\0';
        return strdup(buffer); 
    } else {
        return NULL; 
    }
}



long getVirtualMemorySize(int pid) {
    char path[1024];
    sprintf(path, "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Failed to open /proc/[pid]/status");
        return -1;
    }

    char line[128];
    long virtual_memory = -1;  

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            
            char *value_str = strchr(line, ':');
            if (value_str != NULL) {
                virtual_memory = atol(value_str + 1) * 1024;  
                break;  
            }
        }
    }

    fclose(file);
    return virtual_memory;
}

void proclore(int pid) {
    if (pid == 0) {
        
        pid = getpid();
    }

   
    if (pid <= 0) {
        printf("Invalid PID\n");
        return;
    }

    
    int status;
    if (waitpid(pid, &status, WNOHANG) == -1) {
        perror("waitpid");
        if (errno == ECHILD) {
            printf("No child processes with PID %d\n", pid);
        } else {
            printf("Error waiting for PID %d\n", pid);
        }
        return;
    }

 
    const char *status_str = getStatusString(status);

    char process_type = '+';
    for (int i = 0; i < num_background_processes; i++) {
        if (background_processes[i].pid == pid) {
            process_type = ' ';
            break;
        }
    }

 
    int process_group = getpgid(pid);
    long virtual_memory = getVirtualMemorySize(pid);

  
    char *exe_path = getExecutablePath(pid);

    if (exe_path != NULL) {
    
        printf("pid : %d\n", pid);
        printf("process status : %c%s\n", process_type, status_str);
        printf("Process Group : %d\n", process_group);
        printf("Virtual memory : %ld bytes\n", virtual_memory);
        printf("executable path : %s\n", exe_path);
        free(exe_path);
    } else {
        printf("Failed to retrieve executable path for PID %d.\n", pid);
    }
}