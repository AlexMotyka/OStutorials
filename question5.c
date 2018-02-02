#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct grade {
  int student_id;
  int mark;
};

void grade_student(struct grade *grades, int num_students) {
  FILE *fp;
  fp = fopen("grades.txt", "w");

  int sum = 0;
  for (int i = 0; i < num_students; i++) {
    sum += grades[i].mark;
    fprintf(fp, "%d,%d\n", grades[i].student_id, grades[i].mark);
  }

  float avg = (float)sum / num_students;
  int sum1 = 0;
  for (int i = 0; i < num_students; i++) {
      sum1 += pow((grades[i].mark - avg), 2);
  }
  float variance = sum1 / (float)num_students;
  float std_dev = sqrt(variance);
  fprintf(fp, "Average: %f, Standard Deviation: %f\n", avg, std_dev);
}

int main(void)
{
    //size for our pointers
    size_t length;
    size_t num;
    length = 256;

    //allocate memory
    char *professor = (char*) calloc (length,sizeof(char));
    if (professor == NULL)
      exit(1);

    //prompt professor
    printf("Enter your name: ");
    scanf("%s", professor);

    printf("Enter the number of students to mark: ");
    int num_students;
    scanf("%d", &num_students);

    num = num_students;
    struct grade *grades = malloc(num);
    if (grades == NULL)
      exit(1);

    for (int j = 0; j < num_students; j++) {
      printf("Enter student id: \n");
      scanf("%d", &grades[j].student_id);
      printf("Enter mark: \n");
      scanf("%d", &grades[j].mark);
    }

    grade_student(grades, num_students);

    free(professor);
    professor = NULL;
    free(grades);
    grades = NULL;

    return 0;
}
