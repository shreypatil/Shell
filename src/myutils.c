#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h>
#include "procll.h"
#include "execute.h"
#include "colors.h"
#include "sighan.h"
#include "libs.h"
#include "gvars.h"

// length of command
int getlen(char* command[]) {
    int i = 0;
    while(command[i] != NULL) 
        i++;
    return i;
}


// ERROR CHECK fro Dup
void dup_check(int dup_ret) {
    if(dup_ret < 0) {
        fprintf(stderr, "Could not redirect.");
    }
}


// reset STDIN_FILENO and STDOUT_FILENO
void reset_stdfno() {
    usleep(IO_SLEEP_TIME);
    if (dup2(ORG_STDOUT, STDOUT_FILENO) < 0) {
        perror("dup ");
        return ;
    }
    if (dup2(ORG_STDIN, STDIN_FILENO) < 0) {
        perror("dup ");
        return ;
    }
    usleep(IO_SLEEP_TIME);
}


// get subcommand
int format_redir_cmd(char* dest[], char* const command[], int start, int end) {
    int i;
    for(i = start; i < end; i++)  
        dest[i - start] = command[i];
    dest[i-start] = NULL;
}

// Get string form command
void get_cmd_str(char* dest, char* const command[], int cmd_wc) {
    strcpy(dest, command[0]);
    for(int i=1; i<cmd_wc; i++) {
        strcat(dest, command[i]);
        strcat(dest, " ");
    }
}
