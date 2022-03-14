#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


struct termios orig_termios;

void die(const char *s) {
    perror(s);
    exit(1);
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) perror("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) perror("tcsetattr");
}


void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

int get_input(char* buffer){
    char c;
    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        printf("Prompt>");
        memset(buffer, '\0', 128);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (buffer[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        buffer[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    buffer[pt++] = c;
                    for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                        printf(" ");
                    }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                buffer[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        printf("\nInput Read: [%s]\n", buffer);
    }
    return 0;
}