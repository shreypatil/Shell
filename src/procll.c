
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

#include "execute.h"

#include "procll.h"

#include "gvars.h"


void add_proc(pid_t pid, char* proc_name) {
    char *name = (char*) malloc(64 *  sizeof(char));
    strcpy(name, proc_name);

    if (processes == NULL) {
        processes = ( proc*)malloc(sizeof( proc));

        processes->pid = pid;
        processes->name = name;
        processes->next = NULL; 
        processes->status = 1;
    }
    else if(strcmp(name, processes->name) <= 0) {
        proc* temp = (proc*)malloc(sizeof(proc));
        temp->name = name;
        temp->pid = pid;
        temp->status = 1;
        temp->next = processes;
        processes = temp;
    }
    else {
        proc *curr = processes;
        while(curr->next != NULL & (strcmp(name, curr->name) > 0)) 
            curr = curr->next;
        

        proc* temp = (proc*)malloc(sizeof(proc));
        temp->name = name;
        temp->pid = pid;
        temp->status = 1;
        curr->next = temp->next;
        curr->next = temp;
    }
}
    


void remove_pid(pid_t pid) {
    proc* curr = processes, *prev = NULL;
    while(pid != curr->pid) {
        prev = curr;
        curr = curr->next;

        // Process not found
        if(curr == NULL) {
            printf("%d : No such PID", pid);
            return;
        }
    }

    // pid is of first process
    if(prev == NULL) processes = processes->next;

    else  prev->next = curr->next;
    
}


proc* get_by_ix(int ix) {
    if(ix <= 0) return NULL;
    proc* curr = processes;
    while(--ix > 0) {
        curr = curr->next;
        if(curr == NULL) return NULL;
    }
    return curr;
}

proc* get_by_pid(pid_t pid) {
    proc* curr = processes;
    while(curr->pid != pid) {
        curr = curr->next;
        if(curr == NULL) return NULL;
    }
    return curr;
}

void set_running(int ix){
    if(ix <= 0) return;
    proc* curr = processes;
    while(--ix > 0) {
        curr = curr->next;
        if(curr == NULL) return;
    }
    curr->status = 1;
}

void set_sleeping(int ix) {
    if(ix <= 0) return;
    proc* curr = processes;
    while(--ix > 0) {
        curr = curr->next;
        if(curr == NULL) return;
    }
    curr->status = 0;
}