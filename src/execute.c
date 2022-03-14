
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
#include "procll.h"
#include "fg.h"
#include "bg.h"
#include "sighan.h"
#include "jobs.h"
#include "sig.h"
#include "gvars.h"
#include "myutils.h"
#include "replay.h"


// Executes Built-in Commands
int exec_bic(char* const command[], int cmd_wc) {
    if(strcmp(command[0], "echo") == 0) {
        echo(command, cmd_wc);
        return 0;
    }
    
    else if(strcmp(command[0], "pwd") == 0) {
        printwd(command, cmd_wc);
        return 0;
    }
    
    else if(strcmp(command[0], "cd") == 0) {
        cd(command, cmd_wc);
        return 0;
    }
    
    else if(strcmp(command[0], "ls") == 0) {
        ls(command, cmd_wc);
        return 0;
    }
    
    else if(strcmp(command[0], "pinfo") == 0) {
        pinfo(command, cmd_wc);
        return 0;
    }
    else if(strcmp(command[0], "fg") == 0) {
        fg(command, cmd_wc);
        return 0;
    }
    else if(strcmp(command[0], "bg") == 0) {
        bg(command, cmd_wc);
        return 0;
    }
    else if(strcmp(command[0], "jobs") == 0) {
        jobs(command, cmd_wc);
        return 0;
    }
    else if(strcmp(command[0], "sig") == 0) {
        sig(command, cmd_wc);
        return 0;
    }
    else if(strcmp(command[0], "history") == 0) {
        //history(command, cmd_wc);
        return 0;
    }
    
    return 1;
}



int is_bg_proc(char* command) {
    int cmd_wc = strlen(command);
    int is_bg = 0;
    for(int i = cmd_wc-1; i > 0; i--) {
        if(command[i] == '&') {
            command[i] = ' ';
            return 1;
        } 
        else if(cmd_wc != ' ' | cmd_wc != '\n' | command[i] != '\t') {
            return 0;
        }
    }
}



// calls commands once redirects are handled    
int execute_in(char* const command[], int cmd_wc, int is_bg) {
    if(exec_bic(command, cmd_wc) != 1) return 0;
        
    pid_t ppid = getpid();
    pid_t pid = fork();
        
    if(pid == 0) {
        
        if(is_bg) {
            setpgid(0,0);
            add_proc(getpid(), command[0]);
        } else {
            //get_cmd_str(FG_PROC_NAME, command, cmd_wc);
            FG_PID = getpid();
        }
        if(strcmp(command[0], "replay") == 0) {
            replay(command, cmd_wc);
        }
        
        else if(execvp(command[0], command) == -1) {
            printf("%s : Invalid command\n", command[0]);
            exit(0);
        }
            
    }
    else{
        
        int st_loc = 0;
        
        if(is_bg == 0) {
            //kill(pid, SIGCONT);
            waitpid(pid, &st_loc, WUNTRACED);
                
        }
        else {
            printf("Process %d created\n", pid);
            char* proc_nm = (char*) malloc(128);
            get_cmd_str(proc_nm, command, cmd_wc);
            add_proc(pid, proc_nm);
            free(proc_nm);
            signal (SIGCHLD, proc_exit);
        }
    }

}

// handles redirects and calls commands
void redir_exec(char* const command[], int cmd_wc, int is_bg) {
    int redir_pos[cmd_wc];
    int redir_count = 0;

    // count redirects
    for(int i=0; i< cmd_wc; i++) {
        if((strcmp(command[i], "|") == 0) | (strcmp(command[i], "<") == 0) | (strcmp(command[i], ">") == 0) | (strcmp(command[i], ">>") == 0)) {
            redir_pos[redir_count++] = i;
        }
    }
    
    // temp vars to redirect
    int rw_fds[2];
    rw_fds[0] = STDIN_FILENO;
    rw_fds[1] = STDOUT_FILENO;
    int pipe_fds[2];
    int pipe_fds_prev[2];
    int org_stdin = dup(STDIN_FILENO);
    
    if(redir_count == 0) {
        execute_in(command, cmd_wc, is_bg);
        return;
    }
    char* subcommandf[MAX_CMDS];
    
    if(redir_count == 1) {
        if(strcmp(command[redir_pos[0]], "<") == 0) {
            rw_fds[0] = open(command[redir_pos[0] + 1], O_RDONLY);
            if(rw_fds[0] < 0) {
                perror("open ");
                return;
            }
            dup_check(dup2(rw_fds[0], STDIN_FILENO));
            usleep(IO_SLEEP_TIME);
            close(rw_fds[0]);
            format_redir_cmd(subcommandf, command, 0, redir_pos[0]);
            execute_in(subcommandf, getlen(subcommandf), is_bg);
            reset_stdfno();
        }
        else if((strcmp(command[redir_pos[0]], ">") == 0) | (strcmp(command[redir_pos[0]], ">>") == 0)) {
            rw_fds[1] = (strcmp(command[redir_pos[0]], ">") == 0) ? 
                open(command[redir_pos[0] + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) : 
                open(command[redir_pos[0] + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(rw_fds[1] < 1) {
                perror("open ");
                return;
            }
            dup_check(dup2(rw_fds[1], STDOUT_FILENO));
            usleep(IO_SLEEP_TIME);
            close(rw_fds[1]);
            format_redir_cmd(subcommandf, command, 0, redir_pos[0]);
            execute_in(subcommandf, getlen(subcommandf), is_bg);
            usleep(IO_SLEEP_TIME);
            reset_stdfno();          
        }
        else if(strcmp(command[redir_pos[0]], "|") == 0) {
            pipe(pipe_fds);
            usleep(IO_SLEEP_TIME);
            dup_check(dup2(pipe_fds[1], STDOUT_FILENO));
            usleep(IO_SLEEP_TIME);
            close(pipe_fds[1]);
            format_redir_cmd(subcommandf, command, 0, redir_pos[0]);
            execute_in(subcommandf, getlen(subcommandf), is_bg);

            reset_stdfno();

            dup_check(dup2(pipe_fds[0], STDIN_FILENO));
            usleep(IO_SLEEP_TIME);
            format_redir_cmd(subcommandf, command, redir_pos[0] + 1, cmd_wc);
            execute_in(subcommandf, getlen(subcommandf), is_bg);
            close(pipe_fds[0]);

            reset_stdfno();
        }
    }
    else {
        for(int j=0; j<redir_count; j++) { 
            if(j == 0 && (strcmp(command[redir_pos[0]], "<") == 0)) {
                if((strcmp(command[redir_pos[1]], ">") == 0) | (strcmp(command[redir_pos[1]], ">>") == 0)) {
                    // set Input File
                    rw_fds[0] = open(command[redir_pos[0] + 1], O_RDONLY);
                    if(rw_fds[0] < 0) {
                        perror("open ");
                        reset_stdfno();
                        return;
                    }
                    dup_check(dup2(rw_fds[0], STDIN_FILENO));
                    usleep(IO_SLEEP_TIME);
                    close(rw_fds[0]);

                    // set Output File
                    rw_fds[1] = (strcmp(command[redir_pos[1]], ">") == 0) ? 
                        open(command[redir_pos[1] + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) : 
                        open(command[redir_pos[1] + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if(rw_fds[1] < 1) {
                        perror("open ");
                        reset_stdfno();
                        return;
                    }
                    dup_check(dup2(rw_fds[1], STDOUT_FILENO));
                    usleep(IO_SLEEP_TIME);
                    close(rw_fds[1]);

                    // format and Execute Command
                    format_redir_cmd(subcommandf, command, 0, redir_pos[0]);
                    execute_in(subcommandf, getlen(subcommandf), is_bg);

                    reset_stdfno();
                }
                else if (strcmp(command[redir_pos[1]], "|") == 0) {
                    // set input file
                    rw_fds[0] = open(command[redir_pos[0] + 1], O_RDONLY);
                    if(rw_fds[0] < 0) {
                        perror("open ");
                        reset_stdfno();
                        return;
                    }
                    dup_check(dup2(rw_fds[0], STDIN_FILENO));
                    close(rw_fds[0]);


                    // Create pipe and set out file(pipe)
                    pipe(pipe_fds);
                    usleep(IO_SLEEP_TIME);
                    dup_check(dup2(pipe_fds[1], STDOUT_FILENO));
                    usleep(IO_SLEEP_TIME);
                    close(pipe_fds[1]);

                    // format and execute 1st command
                    format_redir_cmd(subcommandf, command, j==0 ? 0 : (redir_pos[j-1] + 1), redir_pos[j]);
                    execute_in(subcommandf, getlen(subcommandf), is_bg);

                    reset_stdfno();

                    // clone input stream for next iteration
                    rw_fds[0] = dup(pipe_fds[0]);
                    close(pipe_fds[0]);

                    j++;
                    
                    // if last redirect, execute last command
                    if(j == redir_count - 1) {
                        reset_stdfno();
                        dup_check(dup2(rw_fds[0], STDIN_FILENO));
                        usleep(IO_SLEEP_TIME);
                        format_redir_cmd(subcommandf, command, redir_pos[j] + 1, cmd_wc);
                        execute_in(subcommandf, getlen(subcommandf), is_bg);
                        close(rw_fds[0]);
                    }
                    reset_stdfno();

                }
            }
            
            else if((strcmp(command[redir_pos[j]], ">") == 0) | (strcmp(command[redir_pos[j]], ">>") == 0)) {
                // Set read to rw_fds[0] (write location of previous redirect)
                dup_check(dup2(rw_fds[0], STDIN_FILENO));
                usleep(IO_SLEEP_TIME);
                close(rw_fds[0]);

                // Set write to specifies path.
                rw_fds[1] = (strcmp(command[redir_pos[j]], ">") == 0) ? 
                    open(command[redir_pos[j] + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) : 
                    open(command[redir_pos[j] + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(rw_fds[1] < 1) {
                    perror("open ");
                    reset_stdfno();
                    return;
                }
                dup_check(dup2(rw_fds[1], STDOUT_FILENO));
                usleep(IO_SLEEP_TIME);
                close(rw_fds[1]);

                // format and execute command
                format_redir_cmd(subcommandf, command, redir_pos[j - 1] + 1, redir_pos[j]);
                execute_in(subcommandf, getlen(subcommandf), is_bg);

                reset_stdfno();

            }
            
            else if((strcmp(command[redir_pos[j]], "|") == 0)) {
                // Set input file to output of last redirect
                fprintf(stderr, "found |\n");
                
                dup_check(dup2(rw_fds[0], STDIN_FILENO));
                usleep(IO_SLEEP_TIME);
                close(rw_fds[0]);

                // Create pipe and set this pipe as o/p file
                pipe(pipe_fds);
                usleep(IO_SLEEP_TIME);
                dup_check(dup2(pipe_fds[1], STDOUT_FILENO));
                usleep(IO_SLEEP_TIME);
                close(pipe_fds[1]);

                

                // format and execute command
                format_redir_cmd(subcommandf, command, j==0 ? 0 : (redir_pos[j-1] + 1), redir_pos[j]);
                execute_in(subcommandf, getlen(subcommandf), is_bg);

                // clone input stream for next iteration
                rw_fds[0] = dup(pipe_fds[0]);
                close(pipe_fds[0]);
                
                // if last redirect, execute last command
                if(j == redir_count - 1) {
                    reset_stdfno();
                    dup_check(dup2(rw_fds[0], STDIN_FILENO));
                    usleep(IO_SLEEP_TIME);
                    format_redir_cmd(subcommandf, command, redir_pos[j] + 1, cmd_wc);
                    fprintf(stderr, "%s %s %s", subcommandf[0], subcommandf[1], subcommandf[2]);
                    execute_in(subcommandf, getlen(subcommandf), is_bg);
                    close(rw_fds[0]);
                }
                reset_stdfno();
            }
        }    
    }
}



// called through main
// formats string and calls redirect
int execute(char* command) {

    char *cmd_args_ptr;
    char* commandf[MAX_CMDS];

    int is_bg = is_bg_proc(command);


    // Tokenise by " ".
    char* cmd_args = strtok_r(command, " ", &cmd_args_ptr);
    int cmd_wc;
    int repeat_count = 1;

    if(strcmp(cmd_args, "repeat") == 0) {
        repeat_count = atoi(strtok_r(NULL, " ", &cmd_args_ptr));
        cmd_args = strtok_r(NULL, " ", &cmd_args_ptr);
    }
    
    for(cmd_wc = 0; cmd_wc < (MAX_CMDS-1); cmd_wc++) {
        commandf[cmd_wc] = cmd_args;
        cmd_args = strtok_r(NULL, " ", &cmd_args_ptr);
        if(cmd_args == NULL) {
            cmd_wc++;
            break;
        }
    }
    commandf[cmd_wc] = NULL;


    // check redirects and execute
    for(int i = 0; i<repeat_count; i++) {
        redir_exec(commandf, cmd_wc, is_bg);
        
    }
}   


