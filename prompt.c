#include "headers.h"

void prompt(char *prev_command, int prev_command_duration) {
    char USER[4096];
    char SYS[4096];
    char cwd[MAX_PATH_LEN];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        getlogin_r(USER, sizeof(USER));
        gethostname(SYS, sizeof(SYS));

       
        char prompt_str[16384];
        
        if (prev_command != NULL && prev_command_duration!=0 && prev_command_duration>2) {
            snprintf(prompt_str, sizeof(prompt_str), "<%s@%s %s : %ds> ", USER, SYS,prev_command, prev_command_duration);
        } else {
            snprintf(prompt_str, sizeof(prompt_str), "<%s@%s> ", USER, SYS);
        }

        

        printf("%s", prompt_str);
    } else {
        perror("getcwd error");
    }
}
