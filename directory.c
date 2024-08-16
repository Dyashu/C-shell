#include "headers.h"

void get_relative_path(const char* cwd, const char* home_dir, char* rel_path) {
    if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
        snprintf(rel_path, MAX_PATH_LEN, "~%s", cwd + strlen(home_dir));
    } else {
        snprintf(rel_path, MAX_PATH_LEN, "%s", cwd);
    }
}

