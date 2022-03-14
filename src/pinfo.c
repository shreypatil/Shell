
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include "libs.h"
#include "gvars.h"

//#define PATH_LEN 64
//#define PINFO_BUFF_SZ 1024 * 128

int pinfo(char* const command[], int cmd_wc) {
    pid_t pid;
    if(cmd_wc == 2) {
        pid = atoi(command[1]);
    }
    else if (cmd_wc == 1) {
        pid = getpid();
    
    } else {
        printf("pinfo : Too many arguments\n");
        return 1;
    }

    char* proc_stf = (char*) malloc(PATH_LEN * sizeof(char));
    sprintf(proc_stf, "/proc/%d/status", pid);

    //Open /proc/<pid>/status
    FILE* fp = fopen(proc_stf, "r");
	if(fp == NULL){
		perror("pinfo:");
		return 1;
	}
    //Print PID
    printf("PID:\t %d\n", pid);
	char* proc_st_buff = (char*) malloc(PINFO_BUFF_SZ * sizeof(char));

	fread(proc_st_buff, PINFO_BUFF_SZ, sizeof(char), fp);
    fclose(fp);

    //Print State
    char* state_ptr = strstr(proc_st_buff, "State");
    char* temp = state_ptr;
    do {
        printf("%c", *temp);
        temp++;
    } while(*temp != '\n');
    if(pid == getpid()) printf("+");
    printf("\n");

    char* vmsz_ptr = strstr(proc_st_buff, "VmSize");
    if(vmsz_ptr == NULL) printf("Unable to fetch VmSize of Zombie Process");
    else {
        do {
        printf("%c", *vmsz_ptr);
        vmsz_ptr++;
    } while(*vmsz_ptr != '\n');
    printf("\n");
    }
    
    
    sprintf(proc_stf, "/proc/%d/exe", pid);
    if (readlink(proc_stf, proc_st_buff, sizeof(proc_st_buff)) < 0)
        perror("pinfo \n");
    else {
        printf("Executable Path : "); 
        char* exec_path = proc_st_buff;
        do {
            printf("%c", *exec_path);
            exec_path++;
        } while(*exec_path != '\n');
        printf("\n");
    }
    free(proc_st_buff);
    
}