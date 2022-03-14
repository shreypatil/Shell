  
#ifndef _LS_
#define _LS_


int list_ls();

void ls_format(char* dest, char* arg);

void print_entry(char* name, struct stat sb, int l_flag);

void ls(char* const command[], int cmd_len);

#endif