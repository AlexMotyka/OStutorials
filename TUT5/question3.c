#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<pthread.h>


typedef struct {
  char name[20];
  int student_id;
  int mark;
} student;


void* bellcurve(void *arg) {
    student* data = (student*) arg;

    printf("name is %s, mark is %d, id is %d \n", data->name, data->mark, data->student_id);
    

    return NULL;

}

int main()
{
    pthread_t tid[5];
   
    int num_students;
    
    printf("Enter the number of students to mark: \n");
    scanf("%d", &num_students);

    student* grades = malloc(sizeof(student));
    
     for (int j = 0; j < num_students; j++) {
      printf("Enter student name: \n");
      scanf("%s", &grades[j].name);
      printf("Enter student id: \n");
      scanf("%d", &grades[j].student_id);
      printf("Enter mark: \n");
      scanf("%d", &grades[j].mark);

      printf("name is %s, mark is %d, id is %d \n", grades[j].name, grades[j].student_id, grades[j].mark);
    }
    

    for (int k=0; k<num_students; k++){
        
        pthread_create(&tid[k], NULL, bellcurve, grades);
    }

    for (int i = 0; i < num_students; i++) {
        pthread_join(tid[i], NULL);
    }

    
    free(grades);
    grades = NULL;

    return 0;
}