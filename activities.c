#include "headers.h"


void listActivities() {
    printf("Activities:\n");

    for (int i = 0; i < num_background_processes; i++) {
        printf("%d : %s - %s\n", background_processes[i].pid, background_processes[i].command,
               background_processes[i].completed ? "Running" : "Stopped");
    }
}


void executeActivitiesCommand() {
    listActivities();
}
