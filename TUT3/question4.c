#include <stdlib.h>
#include <stdio.h>
int main(void)
{
    //declare pointers
    char *professor;
    int *student_ids;
    int *grades;
    int i = 0;

    //size for our pointers
    size_t length;
    size_t num;
    length = 256;



    //allocate memory
    professor = (char*) calloc (length,sizeof(char));

    //prompt professor 
    printf("Enter your name: ");
    scanf("%s", professor);

    printf("Enter the number of students to mark: ");
    scanf("%d", &i);

    //store the number of students as our size for the following pointers
    num = i;

    student_ids = (int *) malloc(num);

    if (student_ids == NULL) {
        /* Memory could not be allocated, the program should
        handle the error here as appropriate. */
        printf("Memory allocation for student ids failed!\n");
        exit(1);
    } else {
        printf("Success for student ids! Freeing mem now.\n");
        free(student_ids);      
        student_ids = NULL; 
    }

    grades = (int *) malloc(num);

    if (grades == NULL) {
        /* Memory could not be allocated, the program should
        handle the error here as appropriate. */
        printf("Memory allocation for grades failed!\n");
        exit(1);
    } else {
        printf("Success for grades! Freeing mem now.\n");
        free(grades);      
        grades = NULL; 
    }
}
