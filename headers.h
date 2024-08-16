#ifndef HEADERS_H_
#define HEADERS_H_


#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <math.h> 
#include "prompt.h"
#include <ctype.h>
#include <fcntl.h>



#define MAX_BACKGROUND_PROCESSES 20 
#define MAX_COMMANDS 20 
#define MAX_TOKENS 20   
#define MAX_PAST_EVENTS 15
#define MAX_EVENT_LENGTH 20
#define MAX_PATH_LEN 8196
#define MAX_COMMAND_LENGTH 100
#define MAX_FILE_CONTENT 8194
#define MAX_SEARCH_LEN 40
#define MAX_FLAGS_LEN 5
#define MAX_LEN 4096

struct PastEvent {
    char event[MAX_EVENT_LENGTH];
    struct PastEvent* next;
};
 
struct BackgroundProcess {
    int pid;
    char command[MAX_COMMAND_LENGTH];
    int completed;
    clock_t start_time; 
    clock_t end_time;  
    int exit_status; 
};

extern struct BackgroundProcess background_processes[MAX_BACKGROUND_PROCESSES];
extern int num_background_processes;
extern int foreground_pid;



// // In headers.h
// struct ProcessInfo {
//     int pid;
//     char command[MAX_COMMAND_LENGTH];
//     int completed; // 1 if the process has completed, 0 otherwise
// };


// extern struct ProcessInfo processes[MAX_BACKGROUND_PROCESSES];
// extern int num_processes; // Declare the variable here


// void prompt();


void prompt(char *prev_command, int prev_command_duration) ;
void proclore(int pid);
void print_background_messages();
void addPastEvent(struct PastEvent** head, const char* event);
void printPastEvents(struct PastEvent* head);
void clearPastEvents(struct PastEvent** head);
void executePastEvents(int index, struct PastEvent** head);
void get_relative_path(const char* cwd, const char* home_dir, char* rel_path);
char *getExecutablePath(int pid);
const char *getStatusString(int status);

//seek
void seekInDirectory(const char *targetDir, const char *search, int isDirectoryFlag, int isFileFlag, int exactMatchFlag);
int isDirectory(const char *path);
// int expand_tilde(char *expanded_path, const char *path);


void handle_output_redirection(char *filename, char *command_tokens[], int num_tokens );

void handle_append_redirection(char *filename, char *command_tokens[], int num_tokens);
void handle_input_redirection(char *filename, char *command[], int num_tokens) ;
void executeCommand(char *command);
void handle_pipes(char *command);
void executeActivitiesCommand();
void listActivities();
void checkBackgroundProcesses();
long getVirtualMemorySize(int pid);

//signals
void sendSignalToProcess(int pid, int signalNumber);
// void handleCtrlC(int signum);
void handleCtrlZ(int signum);
void handleCtrlD();
void handlePingCommand(char *arg1,char *arg2);
void updateProcessStatus(int pid, int signalNumber);

//neonate
void printMostRecentPID();
void enableRawMode();
void disableRawMode();
void die1(const char *s);






//manpage
void fetchAndDisplayManPage(const char* command_name);

//fg-bg
struct ProcessInfo {
    int pid;
    char command[MAX_COMMAND_LENGTH];
    int completed; // Flag to indicate if the process has completed
};

extern struct ProcessInfo processes[MAX_BACKGROUND_PROCESSES];
extern int num_processes;

void bringToForeground(int pid);
void resumeInBackground(int pid);
void addProcess(int pid, const char* command);
void markProcessAsCompleted(int pid);



#endif