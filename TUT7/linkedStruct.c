#include <stdio.h>
#include <stdlib.h>

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
        printf("Hello %s\n", current->process->name);
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
    
    proc_t * init;
    init = malloc(sizeof(proc_t));

    proc_t * second = malloc(sizeof(proc_t));

    

    

    strcpy(init[0].name, "bob");
    init[0].priority = 1;
    init[0].pid = 2;
    init[0].runtime = 3;

    strcpy(second[0].name, "dave");
    second[0].priority = 5;
    second[0].pid = 3;
    second[0].runtime = 7;

    push(init);
    push(second);
    print_list(head);
    
}