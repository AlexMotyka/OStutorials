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


// Main starts here
int main(){
    // Create instance of linked list called queue
    queue_t * queue = malloc(sizeof(queue_t));
    queue->next = NULL;

    // Read the file and add
    FILE * fp = fopen ("processes_q5.txt", "r");
    proc_t * temp = malloc(sizeof(proc_t));
    char str1[20];
    int one, two = 0;
    for(int i = 0; i < 10; i++){
        fscanf(fp, "%s%*c %d%*c %d", str1, &one, &two);
        strtok(str1, ",\n");
        strcpy(temp[i].name, str1);
        temp[i].priority = one;
        temp[i].pid = 0; // pid is set to 0
        temp[i].runtime = two;
        push(&temp[i]); // add
        printf("%s %d %d %d\n", temp[i].name, temp[i].priority, temp[i].pid, temp[i].runtime);
    }
    fclose(fp);

    for(int i = 0; i < 10; i++){
        if (temp[i].priority == 0) { // If priority is 0
          pid_t pid;
          int status;
          pid = fork();

          if (pid < 0) { // Fail
            status = -1;
            exit(1);
          }

          else if (pid == 0) { // Child
            delete_name(temp[i].name); // use delete_name() to remove the process struct
            execv("./process", NULL);
            exit(0);
          }

          else { // Parent
            sleep(temp[i].runtime); // Run the process for runtime second
            kill(pid, SIGINT);
            waitpid(pid, &status, 0);
            if (status == 0) {
              temp[i].pid = pid; // Set pid
              printf("Name: %s | Priority: %d | PID: %d | Runtime: %d\n", temp[i].name, temp[i].priority, temp[i].pid, temp[i].runtime);
            }
          }
        }

        else { // If priority is NOT 0
          pid_t pid;
          int status;
          pid = fork();

          if (pid < 0) { // Fail
            status = -1;
            exit(1);
          }

          else if (pid == 0) { // Child
            pop(); // use pop() to remove each item
            execv("./process", NULL);
            exit(0);
          }

          else { // Parent
            sleep(temp[i].runtime); // Run the process for runtime second
            kill(pid, SIGINT);
            waitpid(pid, &status, 0);
            if (status == 0) {
              temp[i].pid = pid; // Set pid
              printf("Name: %s | Priority: %d | PID: %d | Runtime: %d\n", temp[i].name, temp[i].priority, temp[i].pid, temp[i].runtime);
            }
          }
        }
    }

    return 0;
}
