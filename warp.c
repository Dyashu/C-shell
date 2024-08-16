#include "headers.h"
#include <pwd.h>

void executeWarp(const char* warp_path, char* current_dir) {
    static char prevdir[MAX_SEARCH_LEN] = ""; 
    if (strcmp(warp_path, "") == 0) {
        
        struct passwd* pw = getpwuid(getuid());
        if (pw) {
            if (chdir(pw->pw_dir) == 0) {
                getcwd(current_dir, MAX_SEARCH_LEN); 
                printf("%s\n", current_dir);
            } else {
                perror("chdir");
            }
        } else {
            perror("getpwuid");
        }
    } else if (strcmp(warp_path, "-") == 0) {
        
        if (prevdir[0] != '\0') {
            char temp[MAX_SEARCH_LEN];
            strcpy(temp, prevdir);
            strcpy(prevdir, current_dir);
            if (chdir(temp) == 0) {
                getcwd(current_dir, MAX_SEARCH_LEN); 
                printf("%s\n", current_dir);
            } else {
                perror("chdir");
            }
        } else {
            printf("No previous directory available.\n");
        }
    } else if (strcmp(warp_path, "..") == 0) {
        
        if (chdir("..") == 0) {
            getcwd(current_dir, MAX_SEARCH_LEN); 
            printf("%s\n", current_dir);
        } else {
            perror("chdir");
        }
    } else {
       
        if (warp_path[0] == '~') {
            struct passwd* pw = getpwuid(getuid());
            if (pw) {
                char dir[MAX_SEARCH_LEN];
                snprintf(dir, sizeof(dir), "%s/%s", pw->pw_dir, warp_path + 1);
                if (chdir(dir) == 0) {
                    getcwd(current_dir, MAX_SEARCH_LEN);
                    printf("%s\n", current_dir);
                } else {
                    perror("chdir");
                }
            } else {
                perror("getpwuid");
            }
        } else {
            if (chdir(warp_path) == 0) {
                getcwd(current_dir, MAX_SEARCH_LEN); 
                printf("%s\n", current_dir);
            } else {
                perror("chdir");
            }
        }
    }
}
