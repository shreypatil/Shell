
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
//#define PATH_LEN 64

#include "libs.h"
#include "gvars.h"

//static char* HOME_DIR;
//static char* PREV_DIR;


//char* HOME_DIR;
//char* PREV_DIR;

char* cd_format(char* arg) {
    if(arg[0] == '~') {
        if(strlen(arg) == 1)
            return HOME_DIR;
        char* ret = (char*) malloc(64 * sizeof(char));
        strcat(ret, HOME_DIR);
        strcat(ret, arg + 2);
        return ret;
    }
    else if(arg[0] == '-') {
        if(strlen(arg) == 1)
            return PREV_DIR;
        char* ret = (char*) malloc(64 * sizeof(char));
        strcat(ret, PREV_DIR);
        strcat(ret, arg + 2);
        return ret;
    }
    else
        return arg;
    
}

void cd(char* const command[], int cmd_len) {
    char* abs_path;
    //printf("cd1");

    if(cmd_len > 2) {
        printf("cd : Too many Arguments\n");
        return;
    }
    if (cmd_len == 1) {
        abs_path = HOME_DIR;
    }
    else {
        abs_path = cd_format(command[1]);
    }
    char* prev = (char*) malloc(PATH_LEN * sizeof(char));
    getcwd(prev, PATH_LEN);

    if(chdir(abs_path) == 0) {
        strcpy(PREV_DIR, prev);
        free(prev);
    }
    else {
        perror("cd ");
    }
}
