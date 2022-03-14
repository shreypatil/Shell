
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


void fg(char* const command[], int cmd_len) {
    if (cmd_len != 2) {
        printf("bg : Invalid number of arguments\n");
        return;
    }

    int job_ix = atoi(command[1]);    

    proc* job = get_by_ix(job_ix);

    if(job == NULL) return;

    pid_t bg_pid = job->pid;

    pid_t bg_pgid = getpgid(bg_pid);

    int status = 0;

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    tcsetpgrp(STDIN_FILENO, bg_pgid);
	kill(bg_pid, SIGCONT);
	
	waitpid(bg_pid, &status, 0);

    sleep(2);
    printf("Hlll");
    sleep(2);
    
    tcsetpgrp(STDIN_FILENO, getpgid(SHELL_PID));

    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);


    
 
    //if(WIFEXITED(status)) return;

}