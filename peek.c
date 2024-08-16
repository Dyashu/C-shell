

#include "headers.h"


char getFileType(mode_t mode) {
    if (S_ISREG(mode)) {
        return 'F'; 
    } else if (S_ISDIR(mode)) {
        return 'D'; 
    } else if (mode & S_IXUSR || mode & S_IXGRP || mode & S_IXOTH) {
        return 'E'; 
    }
    return '?'; 
}


void printColored(const char* text, char type) {
    switch (type) {
        case 'D':
            printf("\x1b[34m%s\x1b[0m\n", text); 
            break;
        case 'E':
            printf("\x1b[32m%s\x1b[0m\n", text); 
            break;
        default:
            printf("%s\n", text); 
    }
}

void executePeek(const char* path, int a_flag, int l_flag) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        fprintf(stderr, "Error opening directory '%s'\n", path);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !a_flag) {
            continue;
        }

        struct stat st;
        char full_path[MAX_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        lstat(full_path, &st);

        char type = getFileType(st.st_mode);
        if (l_flag) {
            struct passwd* user_info = getpwuid(st.st_uid);
            struct group* group_info = getgrgid(st.st_gid);
            printf("%10s", " ");
            printf("%1c", type);
            printColored(entry->d_name, type);

            printf(" %-2ld", (long)st.st_nlink);
            printf(" %-9s", user_info->pw_name);
            printf(" %-9s", group_info->gr_name);
            printf(" %-7lld", (long long)st.st_size);

            char time_str[80];
            strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&st.st_mtime));
            printf(" %-12s", time_str);

            printf(" %s\n", entry->d_name);
        } else {
            printColored(entry->d_name, type);
        }
    }
    closedir(dir);
}
