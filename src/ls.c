
#include <stdlib.h>
#include <stdio.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "colors.h"
#include "gvars.h"
#include <pwd.h>


// Formats Address
void ls_format(char* dest, char* arg) {
    
    if(arg[0] == '~')  
        sprintf(dest, "%s/%s/", HOME_DIR, (arg + 1));
    
    else if(arg[0] != '/') {
        getcwd(dest, 64);
        strcat(dest, "/");
        strcat(dest, arg);
        strcat(dest, "/");
        //printf("%s\n", dest);
    }
    else 
        sprintf(dest, "%s/", arg);    
    
}


//Prints an entry
void print_entry(char* name, struct stat sb, int l_flag) {
    
    if(l_flag == 1) {
        char* to_print = (char*) malloc(256 * sizeof(char));
        // perms
        char type = '-';
        if(sb.st_mode / (64 * 64) == 4) 
            type = 'd';
        else if(sb.st_mode / (64 * 64) == 8)
            type = '-';
        else if(sb.st_mode / (64 * 64) == 10)
            type = 'l';
        to_print[0] = type;
        to_print[1] = *((sb.st_mode & S_IRUSR) ? "r" : "-");
        to_print[2] = *((sb.st_mode & S_IWUSR) ? "w" : "-");
        to_print[3] = *((sb.st_mode & S_IXUSR) ? "x" : "-");
        to_print[4] = *((sb.st_mode & S_IRGRP) ? "r" : "-");
        to_print[5] = *((sb.st_mode & S_IWGRP) ? "w" : "-");
        to_print[6] = *((sb.st_mode & S_IXGRP) ? "x" : "-");
        to_print[7] = *((sb.st_mode & S_IROTH) ? "r" : "-");
        to_print[8] = *((sb.st_mode & S_IWOTH) ? "w" : "-");
        to_print[9] = *((sb.st_mode & S_IXOTH) ? "x" : "-");
        to_print[10] = ' ';
        to_print[12] = ' ';
        
        
        // no of links
        sprintf(to_print + strlen(to_print), "%lu\t", sb.st_nlink); 
        
    
        
        // owner name
        sprintf(to_print + strlen(to_print), "%s\t", getpwuid(sb.st_uid)->pw_name);
        
        
        // group name
        sprintf(to_print + strlen(to_print), "%s\t", getgrgid(sb.st_gid)->gr_name);
        
        
        // size 
        sprintf(to_print + strlen(to_print), "%ld\t", sb.st_size);
        
        
        // date time modofied / created
        struct tm* time_cm = localtime(&(sb.st_mtime));
        sprintf(to_print + strlen(to_print), "%d-%d-%d %2d:%2d ", time_cm->tm_mday,
            time_cm->tm_mon, time_cm->tm_year + 1900, time_cm->tm_hour, time_cm->tm_min);
        
        
        printf("%s", to_print);
        free(to_print);
    }
    
    // Name
    if(sb.st_mode / (64 * 64) == 4)
        blue();
    else if(sb.st_mode / (64 * 64) == 8)
        reset();
    else if(sb.st_mode / (64 * 64) == 10) 
        cyan();
    
    printf("%s  ", name);
    reset();
    if(l_flag == 1) 
        printf("\n");
    
}

// Driver function of ls
// detects flags, interates over entries in dir
void ls(char* const command[], int cmd_len) {
    int l_flag = 0, a_flag = 0, flag_count = 0, addr_counter = 0;
    char *addresses[cmd_len - 1];  
    char* addr;
    char* gp_buffer = (char*) malloc(64 * sizeof(char));

    for(int i = 1; i < cmd_len; i++) {
        if(command[i][0] == '-') {
            l_flag = (strchr(command[i], 'l') != NULL) ? 1 : l_flag;
            a_flag = (strchr(command[i], 'a') != NULL) ? 1 : a_flag;
            flag_count ++;
        } else 
            addresses[addr_counter++] = command[i];       
    }
    
    if(addr_counter == 0) {
        addresses[0] = getcwd(gp_buffer, 64);
        addr_counter = 1;
    }

    
    for(int i = 0; i < addr_counter; i++) {

        addr = malloc(64 * sizeof(char));
        ls_format(addr, addresses[i]);
        
        if(addr_counter > 1)
            printf("%s : \n", addresses[i]);

        struct stat sb;
        if (lstat(addr, &sb) != 0)
        {
            perror(addresses[i]);
            return;
        }

        // If Address points to directory, iterate over it's contents
        if(sb.st_mode & S_IFDIR) {
            DIR* dir = opendir(addr);
            struct dirent* ent_info;

            // print Total
            if(l_flag == 1) 
                printf("total = %ld \n", sb.st_blocks);

            // Iterate over dir
            while (ent_info = readdir(dir)) {
                if (a_flag == 0 && ent_info->d_name[0] == '.') 
                    continue;
                
                // Details of each entry
                struct stat sb2;
                char* entry_addr = (char*) malloc(64 * sizeof(char));
                sprintf(entry_addr, "%s%s", addr, ent_info->d_name);
                lstat(entry_addr, &sb2);
                print_entry(ent_info->d_name, sb2, l_flag);
            }
            printf("\n");
        }

        // If Address points to file / link, print details.
        else {
            print_entry(strrchr(addr, '/'), sb, l_flag);
        }
        free(gp_buffer);
        free(addr);
    }
}