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
#include "procll.h"
#include "echo.h"
#include "execute.h"
#include "printwd.h"
#include "ls.h"
#include "cd.h"
#include "pinfo.h"
#include "colors.h"
#include "gvars.h"
#include "jobs.h"
#include "sig.h"

#define JOBS_SFLAG 2
#define JOBS_RFLAG 1



void jobs(char* const command[], int cmd_len) {
    int flags = 0;

    //sleep(1);
    
    for(int i=0; i<cmd_len; i++) {
        char* temp = command[i];
        if(strcmp(temp, "-s") == 0) flags = flags | JOBS_SFLAG;
        if(strcmp(temp, "-r") == 0) flags = flags | JOBS_RFLAG;
    }
    if(flags == 0) flags = JOBS_RFLAG | JOBS_SFLAG;
    
    

    struct proc* temp_proc = processes;
    for( int i = 0; temp_proc != NULL; i++, temp_proc = temp_proc->next) 
        print_job(i, temp_proc, flags);

}

void print_job(int ix, struct proc* job, int flags) {

    char* proc_fpath = (char*) malloc(PATH_LEN * sizeof(char));
    sprintf(proc_fpath, "/proc/%d/status", job->pid);

    //Open /proc/<pid>/status
    FILE* fp = fopen(proc_fpath, "r");
    char* proc_det = (char*)malloc(256 * sizeof(char));
    fread(proc_det, 256, sizeof(char), fp);
    fclose(fp);
    
    int if_print = 1;
    char* status;
    if(strstr(proc_det, "stopped")) {
        status = "Stopped";
        if((flags & 1) == 0) if_print = 0;
    }
    else if(strstr(proc_det, "zombie")) 
        if_print = 0;   
    else {
        status = "Running";
        if((flags & 2) == 0) if_print = 0;
    }
    
    
    char* pid_details = (char*) malloc(128 * sizeof(char));
    if(if_print) {
        sprintf(pid_details, "[%d]  %s  %s [%d]\n", ix+1, status, job->name, job->pid);
        printf("%s", pid_details);
    }
    
    free(pid_details);
    free(proc_fpath);
    
} 

