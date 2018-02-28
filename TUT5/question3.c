/*wierd memory issues when there is more than 2 students for some reason. 
Not sure why the C gods hate me */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<pthread.h>

int  num_students;


typedef struct {
  char name[20];
  int student_id;
  int mark;
} student;


void* bellcurve(void *arg) {
    
    student* data = (student*) arg;
    for (int j =0; j < num_students; j++) {
      printf("Name is %s, id is %d, bellcurved grade is %f \n", data[j].name, data[j].student_id, (data[j].mark) *1.50);
    }
    
    

    return NULL;

}

int main()
{
    pthread_t tid[5];
   
    
    
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