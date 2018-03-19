#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct proc{
    char parent[256];
    char name[256];
    int priority;
    int memory;
}proc_t;

typedef struct proc_tree{
    struct proc * process;
    struct proc_tree *left;
    struct proc_tree *right;
}proc_tree_t;



proc_tree_t * newNode(proc_t * process){
    proc_tree_t * node = (proc_tree_t*) malloc(sizeof(proc_tree_t));
    node->process = process;
    node->left = NULL;
    node->right = NULL;
    return(node);
}

void preOrder( proc_tree_t *root){
    if (root != NULL){
        printf("Parent: %s | Name: %s | Priority: %d | Memory: %d\n", root->process->parent, root->process->name, root->process->priority, root->process->memory);
        preOrder(root->left);
        preOrder(root->right);
    }
}


int main(void)
{
    
    FILE * fp;
    char str1[256], str2[256];
    int one, two = 0;
   
    fp = fopen ("processes_tree.txt", "r");
    
    proc_t * myproc = malloc(sizeof(proc_t));

    for(int i = 0; i<7; i++){
        fscanf(fp, "%[^,], %[^,], %d, %d\n", str1, str2, &one, &two);
        strcpy(myproc[i].parent, str1);
        strcpy(myproc[i].name, str2);
        myproc[i].priority = one;
        myproc[i].memory = two;
    }

    //initialize root
    proc_tree_t * root = newNode(&myproc[0]);

    root->left = newNode(&myproc[1]);
    root->right = newNode(&myproc[2]);

    root->left->left = newNode(&myproc[3]);
    root->left->right = newNode(&myproc[4]);

    root->right->left = newNode(&myproc[5]);
    root->right->right = newNode(&myproc[6]);

    preOrder(root);

    
}