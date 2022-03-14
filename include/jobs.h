#ifndef _JOBS_
#define _JOBS_

#include "procll.h"

void jobs(char* const command[], int cmd_len);

void print_job(int ix, proc* job, int flags);


#endif