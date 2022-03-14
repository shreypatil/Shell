
#include <stdio.h>
#include <string.h>

#include "libs.h"
#include "gvars.h"

void echo(char *const command[], int cmd_len) {

    for(int i=1; i < cmd_len; i++) 
        printf("%s ", command[i]);
    
    printf("\n");
} 
