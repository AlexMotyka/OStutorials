#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//our proc struct
typedef struct proc{
    char name[256];
    int priority;
    int pid;
    int runtime;
}proc_t;

//our node struct
typedef struct node {
    struct proc * process;
    struct node * next;
} node_t;

//intialize head and current to NULL
node_t *head = NULL;
node_t *current = NULL;



void print_list(node_t * head) {
    //set current to the head
    node_t * current = head;

    //move current through the list until we print the last element
    while (current != NULL) {
        printf("Name: %s | Priority: %d | PID: %d | Runtime: %d\n", current->process->name, current->process->priority, current->process->pid, current->process->runtime);
        current = current->next;
    }
}


void push(proc_t * process) {
    //create a new link in the linked list
    node_t * link = (node_t*) malloc(sizeof(node_t));
    
    //assign the process to the link
    link->process = process;

    //make the link point to the current head
    link->next = head;

    //make the link the new head
    head = link;
    
}

int main(){

    FILE * fp;
    char str1[20];
    int one, two, three = 0;
   
    fp = fopen ("processes.txt", "r");
    
    proc_t * myproc = malloc(sizeof(proc_t));

    for(int i = 0; i<10; i++){
        fscanf(fp, "%s%*c %d%*c %d%*c %d", str1, &one, &two, &three);
        strcpy(myproc[i].name, str1);
        myproc[i].priority = one;
        myproc[i].pid = two;
        myproc[i].runtime = three;

        push(&myproc[i]);
    }
    fclose(fp);
    print_list(head);

    free(myproc);
    myproc = NULL;

    return 1;
    
}