
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
//#include "procll.h"
//#include "fg.h"
//#include "bg.h"
//#include "jobs.h"
//#include "sig.h"
#include "gvars.h"


void proc_exit() {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("\n%d : Process Exited \t %s\n", pid, get_by_pid(pid)->name);
    remove_pid(pid);
}

// Sends proc to bg and stops it.
void sig_ctrlz(int sig) {
    /*
    printf("sig found\n");
    sleep(1);
    printf("FGPID(fg):%d\nSHELLPID:%d", FG_PID, SHELL_PID);
    sleep(1);
    pid_t FG_PID = getpid();
    */
    
    if(FG_PID == SHELL_PID) return;

    add_proc(FG_PID, FG_PROC_NAME);

    setpgid(FG_PID, 0);
    
    kill(FG_PID, SIGSTOP);

    FG_PID = SHELL_PID;

}


// returns to shell
void sig_ctrlc() {

    int pid = getpid();
    int currgpid = getpgid(pid);

    if(getpgid(SHELL_PID) != currgpid)
    {
        kill(pid,SIGINT);
    } 
}