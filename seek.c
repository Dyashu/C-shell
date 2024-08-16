

#include "headers.h"

#include <dirent.h>



int isDirectory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}


void seekInDirectory(const char *targetDir, const char *search, int isDirectoryFlag, int isFileFlag, int exactMatchFlag) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(targetDir);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir))) {
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", targetDir, entry->d_name);

        if (isDirectory(fullPath)) {
            
            seekInDirectory(fullPath, search, isDirectoryFlag, isFileFlag, exactMatchFlag);
        } else if (!isDirectoryFlag && !isFileFlag && strstr(entry->d_name, search)) {
            
            printf("%s/%s\n", targetDir, entry->d_name);
        } else if (isDirectoryFlag && isDirectory(fullPath) && strstr(entry->d_name, search)) {
            
            printf("%s/%s\n", targetDir, entry->d_name);
        } else if (isFileFlag && !isDirectory(fullPath) && strstr(entry->d_name, search)) {
            
            printf("%s/%s\n", targetDir, entry->d_name);
        }
    }

    closedir(dir);
}

