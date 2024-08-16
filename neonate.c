#include "headers.h"
#include <termios.h>


void die1(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die1("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die1("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die1("tcsetattr");
}

void printMostRecentPID() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        
        while (1) {
            sleep(1);
            printf("%d\n", getpid());
            fflush(stdout);
        }
    } else {
        
        int status;
        char c;
        while (1) {
            read(STDIN_FILENO, &c, 1);
            if (c == 'x') {
               
                kill(pid, SIGTERM);
                waitpid(pid, &status, 0);
                break;
            }
        }
    }
}


