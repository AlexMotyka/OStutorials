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

//our queue struct
typedef struct queue {
    struct proc * process;
    struct queue * next;
} queue_t;

//intialize head to NULL
queue_t *head = NULL;


void print_list(queue_t * head) {
    //set pointer to the head
    queue_t * ptr = head;

    //move pointer through the list until we print the last element
    while (ptr != NULL) {
        printf("Name: %s | Priority: %d | PID: %d | Runtime: %d\n", ptr->process->name, ptr->process->priority, ptr->process->pid, ptr->process->runtime);
        ptr = ptr->next;
    }
}

proc_t * pop(){
    
    queue_t * tempLink = head;
    
    head = head->next;
    //printf(head->process->name);
    
    return tempLink->process;
}

proc_t * delete_name(char *name){
    queue_t *ptr = head;
    queue_t *previous = NULL;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list until name found
   while(!(strcmp(ptr->process->name, name) == 0)){

      //if it is the last node in the list
      if(ptr->next == NULL) {
         return NULL;
      } else {
         //store temporary reference to the current link
         previous = ptr;
         //move to the next link
         ptr = ptr->next;
      }
   }

   //found the name now update the link
   if(ptr == head) {
      //change head to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = ptr->next;
   }    

    return ptr->process;
}

proc_t * delete_pid(int pid){
    queue_t *ptr = head;
    queue_t *previous = NULL;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list until pid found
   while(ptr->process->pid != pid) {

      //if it is the last node in the list
      if(ptr->next == NULL) {
         return NULL;
      } else {
         //store temporary reference to the current link
         previous = ptr;
         //move to the next link
         ptr = ptr->next;
      }
   }

   //found the pid now update the link
   if(ptr == head) {
      //change head to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = ptr->next;
   }    

    return ptr->process;
    
}

bool isEmpty() {
   return head == NULL;
}

void push(proc_t * process) {
    //create a new link in the linked list
    queue_t * link = (queue_t*) malloc(sizeof(queue_t));
    
    
    //assign the process to the link
    link->process = process;

    //point the new link to old first node
    link->next = head;

    //link becomes the old head
    head = link;
    
}

int main(){

    FILE * fp;
    char str1[20];
    int one, two, three = 0;
   
    fp = fopen ("processes.txt", "r");
    
    proc_t * myproc = malloc(sizeof(proc_t));

    for(int i = 0; i<10; i++){
        fscanf(fp, "%[^,], %d, %d, %d\n", str1, &one, &two, &three);
        strcpy(myproc[i].name, str1);
        /*if you remove this printf() pop acts wierd with returning values
        I have no idea why though... */
        printf("|%s|\n",str1);
        myproc[i].priority = one;
        myproc[i].pid = two;
        myproc[i].runtime = three;

        push(&myproc[i]);
    }
    
    fclose(fp);
    
    delete_name("emacs");
    delete_pid(12235);
    
    while(!isEmpty()){

        proc_t * temp = malloc(sizeof(proc_t));

        temp = pop();
        printf("Popped-> Name: %s | Priority: %d | PID: %d | Runtime: %d\n", temp->name, temp->priority, temp->pid, temp->runtime);
    }
    
    free(myproc);
    myproc = NULL;

    return 0;
    
}