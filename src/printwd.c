
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


#include "libs.h"
#include "gvars.h"

void printwd(char* const command[], int cmd_wc) {
    if(cmd_wc != 1) {
        printf("Too Many Arguments\n");
        return;
    }

    char* cwd_name = (char*) malloc(64 * sizeof(char));
    size_t len_cwd_name = 64;
    
    getcwd(cwd_name, len_cwd_name);

    printf("%s\n", cwd_name);
    free(cwd_name);
    return;
}