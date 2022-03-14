  
#include <sys/types.h>
#ifndef _PROCLL_
#define _PROCLL_

typedef struct proc {
    pid_t pid;
    char* name;
    int status;
    struct proc* next;
}proc;


void add_proc(pid_t pid, char* proc_name); 

void remove_pid(pid_t pid);

proc* get_by_ix(int ix);

proc* get_by_pid(pid_t pid);

void set_running(int job);

void set_sleeping(int job);

#endif