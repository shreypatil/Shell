#include <stdlib.h>
#include <stdio.h>


static struct hist_cll* HISTORY;

struct hist_node {
    char* cmd;
    struct hist_node* next;
    struct hist_node* prev;
};


struct hist_cll {
    int len;
    int max_len;
    struct hist_node* head;
    struct hist_node* tail;
    struct hist_node* curr;
};


int log_hist(struct hist_cll* history, char* command) {
    if(history->len == 0) {
        struct hist_node* temp = malloc(sizeof(struct hist_node));
        temp->cmd = command;
        history->head = temp;
        history->tail = temp;
        history->curr = temp;
        temp->next = temp;
        temp->prev = temp;
        history->len = 1;
    }
    else if(history->len < history->max_len) {
        struct hist_node* temp = malloc(sizeof(struct hist_node));
        temp->cmd = command;
        temp->next = history->tail;
        history->tail->prev = temp;
        history->head->next = temp;
        temp->prev = history->head;
        history->head = temp;
        history->curr = history->head;
        history->len++;
        printf("%d", history->len);
        printf(">%s\n", history->head->cmd);
        printf(">%s\n", history->head->prev->cmd);
    } else {
        history->tail->cmd = command;
        history->head = history->tail;
        history->tail = history->tail->next;
        history->curr = history->head;
    }
}

int init_history() {
    FILE* history_fp;
    HISTORY = (struct hist_cll*) malloc(sizeof(struct hist_cll));
    HISTORY->max_len = 20;
    HISTORY->len = 0;
  
    history_fp = fopen("command_history", "rw+");
    if(history_fp == NULL) {
      perror("Error opening file");
      return(-1);
    }
    char* cmd = fgets(cmd, 128, history_fp);
    while(cmd != NULL) {
        log_hist(HISTORY, cmd);
        cmd = fgets(cmd, 128, history_fp);
    }
    fclose(history_fp);
    return 0;
}

int save_history(struct hist_cll* history) {
    FILE* history_fp;
    history_fp = fopen("command_history", "w");
    if(history_fp == NULL) {
      perror("Error opening file");
      return(-1);
    }
    history->curr = history->tail;
    for(int i=0; i<history->len; i++) {
        history->curr = history->curr->prev;
        fprintf(history_fp, "%s\n", history->curr->cmd);
    } 
    fclose(history_fp);
    return 0;
}

void print_history(struct hist_cll* history, int n) {
    if(n > history->len) n = history->len;
    history->curr = history->head;
    while(n != 0) {
        printf("%s\n", history->curr->cmd);
    }
}

char* get_prev(struct hist_cll* history) {
    if(history->curr == history->tail) return history->tail->cmd;
    history->curr = history->curr->prev;
    return history->curr->cmd;
}

char* get_next(struct hist_cll* history) {
    if(history->curr == history->head) return history->head->cmd;
    history->curr = history->curr->next;
    return history->curr->cmd;
}

int history(char* const command[], int cmd_wc) {
    if(cmd_wc == 1) {
        print_history(HISTORY, 10);
    } 
    else if(cmd_wc == 2) {
        int len = atoi(command[2]);
        if(len <= 0) printf("Enter positive Number\n");
        print_history(HISTORY, len);
    }
    else {
        printf("Too many arguments\n");
    }
}

