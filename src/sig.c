#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "libs.h"
#include <signal.h>


#include "execute.h"

#include "gvars.h"
#include "procll.h"
#include "jobs.h"
#include "sig.h"



void sig(char* const command[], int cmd_len) {
    if(cmd_len != 3) {
        printf("sig : invalid number of arguments");
        return;
    }
    
    int sig = atoi(command[2]);
    
    int job_ix = atoi(command[1]);   

    proc* job = get_by_ix(job_ix);
    
    pid_t job_pid = job->pid;

    if(kill(job_pid, sig) < 0) 
        printf("sigErr");  
    
}