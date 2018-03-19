#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MEMORY 1024

typedef struct{
	char name[256];
	int priority;
	int pid;
	int address;
	int memory;
	int runtime;
	bool suspended;
} proc;

typedef struct node{
	proc process;
	struct node *next;
} node_t;

proc tempProc;

void push (proc process, node_t * queue){
	node_t *current = queue;
	while(current -> next != NULL) // Find the end spot
		current = current -> next;
	current -> next =  (node_t *) malloc(sizeof(node_t));
	current -> next-> process = process;
	current-> next-> next = NULL; // next next is the end
}

proc * pop(node_t * queue){
	if(queue -> next == NULL) // Empty
		return NULL;
  node_t * nextNode = NULL;
	nextNode = queue -> next -> next;
	node_t * currentNode = queue -> next;
 	tempProc = currentNode-> process;
 	free(currentNode);
	queue -> next = nextNode;
	return &tempProc;
}

void print_list(proc *p){
	if (p != NULL)
		printf("name: %s | priority: %d | pid: %d | memory: %d | runtime: %d\n", p->name, p->priority, p->pid, p->memory, p->runtime);
}

bool allocate_memory(int *memory, int size){
  // TRUE is successful, FALSE is fail
	//printf("Allocating memory for %s\n",tempProc.name);
	sleep(1); // For output apprance
	int allocated = 0;
	int start = 0;
  for(int i = 0; i < size; i++){
		if(memory[i] == 1){
			allocated = 0;
			start = i + 1;
		}
		else
			allocated++;
		if(allocated == tempProc.memory){
			tempProc.address = start;
      // Allocate
			for(int k = 0; k < tempProc.memory; k++)
				memory[k+tempProc.address] = 1;
			return true;
		}
	}
	return false;
}

void deallocate_memory(int *memory){
	//printf("Deallocating memory for %s\n\n",tempProc.name);
	for(int i = 0; i < tempProc.memory;i++)
		memory[i+tempProc.address] = 0;
}

int main(){
  // priority queue
  node_t * priority = NULL;
	priority = malloc(sizeof(node_t));
	priority->next = NULL;

  // secondary queue
  node_t * secondary = NULL;
	secondary = malloc(sizeof(node_t));
	secondary->next = NULL;

  // avail_mem init
  int avail_mem [MEMORY];
	for(int i = 0; i < MEMORY; i++)
		avail_mem[i] = 0;

  // Read file and assign to queue
	FILE *f = fopen("processes_q2.txt","r");
  char str1[20];
  int one, two, three;
  for(int i = 0; i < 10; i++){
      fscanf(f, "%s%*c %d%*c %d%*c %d", str1, &one, &two, &three);
      strtok(str1, ",\n");
      strcpy(tempProc.name, str1);
      tempProc.priority = one;
      tempProc.memory = two;
      tempProc.runtime = three;
      tempProc.pid = 0; // Set to 0
      tempProc.address = 0; // Set to 0
      if(tempProc.priority == 0)
  			push(tempProc,priority); // To priority queue
  		else
  			push(tempProc,secondary); // To secondary queue
  }
	fclose(f);

	// Iterate through priority queue first
	while(priority -> next != NULL){
		tempProc = *pop(priority); // Pop first
		int status;
		pid_t pid;
		pid = fork(); // fork
    if (pid < 0)
			status = -1;
    else if (pid == 0) { // child
			tempProc.pid = getpid();
			tempProc.runtime--;
			allocate_memory(avail_mem, MEMORY); // allocate
			print_list(&tempProc);
			execv("./process", NULL);
			exit(0);
		}
    else { // Parent
			sleep(tempProc.runtime); // Run the process for the specified runtime
			kill(pid, SIGTSTP);
			kill(pid, SIGINT);
			waitpid(pid, &status, 0); // Wait
			deallocate_memory(avail_mem); // Free the memory
		}
	}

  // Iterate through secondary queue
	while(secondary -> next != NULL){
		int status;
		pid_t pid;

		bool mem = true;
		do {
			mem = true;
			tempProc = *pop(secondary); // Next one
			if(!tempProc.suspended){
				mem = allocate_memory(avail_mem, MEMORY); // allocate
				if (mem == false)
					push(tempProc,secondary); // Not enough memory, push
			}
		} while(!mem && !tempProc.suspended);

		if(tempProc.suspended) { // suspended is true
      print_list(&tempProc);
      kill(tempProc.pid, SIGCONT);
      sleep(1); // Run for 1 sec
      kill(tempProc.pid, SIGTSTP);
      if(tempProc.runtime <= 1){ // Only has 1 second left
        kill(tempProc.pid, SIGINT);
        waitpid(tempProc.pid, &status, 0);
        deallocate_memory((int *) &avail_mem); // Free
      }
      else{
        tempProc.runtime--; // decrease timer
        push(tempProc,secondary); // Not finish, push
      }
		}

    else { // suspended is false
      pid = fork();
      if (pid < 0)
        status = -1;
      else if (pid == 0) { // Child
        execv("./process", NULL);
        exit(0);
      }
      else { // Parent
        tempProc.pid = pid;
        print_list(&tempProc);
        tempProc.runtime--;
        tempProc.suspended = true;
        sleep(1);
        kill(pid, SIGTSTP);
        push(tempProc,secondary);
      }
		}
	}

	free(secondary);
	free(priority);

	return 0;
}
