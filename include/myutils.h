#ifndef _MYUTILS_
#define _MYUTILS_

int getlen(char* command[]);

void dup_check(int dup_ret);

void reset_stdfno();

int format_redir_cmd(char* dest[], char* const command[], int start, int end);

void get_cmd_str(char* dest, char* const command[], int cmd_wc);

#endif