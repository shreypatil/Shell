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

//#include "history.c"
//#define PATH_LEN 64

extern int errno;


//static char* HOME_DIR;
//static int HOME_DIR_LEN;
//static char* PREV_DIR;

char* HOME_DIR;
int HOME_DIR_LEN;
char* PREV_DIR;
int SHELL_PID;
int ORG_STDOUT;
int ORG_STDIN;
int FG_PID;
char* FG_PROC_NAME;

proc* processes;

char* remove_home(char* address) {
    //printf("%s\n", HOME_DIR);
    //printf("%s\n", address);
    if(strncmp(HOME_DIR, address, HOME_DIR_LEN) == 0) {
        address = address + HOME_DIR_LEN - 1;
        address[0] = '~';
    }
    return address;
}

void init_home_dir(void) {
    HOME_DIR = (char*) malloc(PATH_LEN * sizeof(char));
    PREV_DIR = (char*) malloc(PATH_LEN * sizeof(char));

    getcwd(HOME_DIR, PATH_LEN);
    //strcat(HOME_DIR, home);
    //strcat(HOME_DIR, "/");
    HOME_DIR_LEN = strlen(HOME_DIR);
    
    //strcat(PREV_DIR, HOME_DIR);
}

void print_sys_name(void) {

    char* usr_name = (char*) malloc(32 * sizeof(char));
    ssize_t len_uname = 32;
    getlogin_r(usr_name, len_uname);

    char* host_name = (char*) malloc(32 * sizeof(char));
    ssize_t len_hname = 32;
    gethostname(host_name, len_hname);

    char* cwd_name = (char*) malloc(PATH_LEN * sizeof(char));
    ssize_t len_cwd_name = PATH_LEN;
    getcwd(cwd_name, len_cwd_name);
    
    green();
    printf("%s@%s:", usr_name, host_name);
    blue();
    printf("%s >>> ", remove_home(cwd_name));
    reset();
    free(usr_name);
    free(host_name);
    free(cwd_name);
}

void inti_stdio_out() {
    ORG_STDOUT = dup(STDOUT_FILENO);
    ORG_STDIN = dup(STDIN_FILENO);
}


int main(void) {
    // init some global vars
    size_t max_line_sz = 128;

    init_home_dir();

    SHELL_PID = getpid();
    printf("SHELL_PID = %d\n", SHELL_PID);
    
    ORG_STDOUT = INT_MAX;
    ORG_STDIN = INT_MAX - 1;
    inti_stdio_out();
    //printf("%d\n", ORG_STDOUT);

    signal(SIGINT, sig_ctrlc);
    signal(SIGTSTP, sig_ctrlz);


    // Program loop
    while(1) {
        //printf("%d\n", SHELL_PID);

        print_sys_name();

        char *input_txt = (char*) malloc(max_line_sz * sizeof(char));
        int n = getline(&input_txt, &max_line_sz, stdin);

        if(input_txt[n-1] == '\n') input_txt[n-1] = '\0';
    
        FG_PROC_NAME = (char*) malloc(128 * sizeof(char));
        strcpy(FG_PROC_NAME, input_txt);
        FG_PID = getpid();

        if(strcmp(input_txt, "exit") == 0) {
            break;
        }

        char *cmd_ptr;
        char *command = strtok_r(input_txt, ";", &cmd_ptr);

        while(command != NULL) {
            execute(command);
            command = strtok_r(NULL, ";", &cmd_ptr);
        } 
        free(FG_PROC_NAME);
        free(input_txt);
    }
}