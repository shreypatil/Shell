#ifndef _GVARS_
#define _GVARS_

#include "procll.h"

#define MAX_CMDS 16
#define PATH_LEN 64
#define PINFO_BUFF_SZ 1024 * 128
#define IO_SLEEP_TIME 50000

extern char* HOME_DIR;
extern char* PREV_DIR;
extern int HOME_DIR_LEN;
extern proc* processes;
extern int SHELL_PID;
extern int ORG_STDOUT;
extern int ORG_STDIN;
extern char* FG_PROC_NAME;
extern int FG_PID;


#endif

