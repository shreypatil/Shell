
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


void bg(char* const command[], int cmd_len) {
    if (cmd_len != 2) {
        printf("bg : Invalid number of arguments\n");
        return;
    }
    int job_ix = atoi(command[1]);   

    proc* job = get_by_ix(job_ix);
    
    pid_t job_pid = job->pid;

    if(kill(job_pid, SIGCONT) < 0) 
        printf("bg "); 

}