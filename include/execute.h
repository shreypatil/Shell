#ifndef _EXECUTE_
#define _EXECUTE_

void reset_stdfno();

void proc_exit();

int exec_bic(char* const command[], int cmd_wc);

int is_bg_proc(char* command);

void redir_exec(char* const command[], int cmd_wc, int is_bg);

int execute_in(char* const command[], int cmd_wc, int is_bg);

int execute(char* command);

int format_redir_cmd(char* dest[], char* const command[], int start, int end);


#endif