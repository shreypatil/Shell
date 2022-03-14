
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
#include "echo.h"
#include "execute.h"
#include "printwd.h"
#include "ls.h"
#include "cd.h"
#include "pinfo.h"
#include "colors.h"
#include "procll.h"
#include "gvars.h"
#include "jobs.h"
#include "sig.h"
#include "myutils.h"


void replay(char* const command[], int cmd_wc) {
    if(strcmp(command[1], "-command") != 0) {
        fprintf(stderr, "replay : Incorrect format\nformat : replay  -command <command> -interval <interval> -period <period>");
        return;
    }
    char* subcommandf[MAX_CMDS];
    int i;

    int interval = atoi(command[cmd_wc - 3]);
    int period = atoi(command[cmd_wc - 1]);
    
    format_redir_cmd(subcommandf, command, 2, cmd_wc - 4);

    pid_t parent_pid = getpid();

    pid_t f_pid = fork();

    if(f_pid == 0) {
        sleep(period);
        usleep(50000);
        kill(parent_pid, SIGKILL);
    }
    else {
        while(1) {
           sleep(interval);
           execute_in(subcommandf, getlen(subcommandf), 0); 
        }
    }

    
}