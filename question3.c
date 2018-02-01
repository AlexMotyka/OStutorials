#include <stdio.h>

struct student {
  int id;
  int age;
  int year;
} student;

void save_student(struct student *s) {
  FILE *fp;
  fp = fopen("students.txt", "a");
  fprintf(fp, "%d,%d,%d\n", s->id, s->age, s->year);
}

int main() {
  printf ("Enter your student id: ");
  scanf("%d", &student.id);
  printf ("Enter your age: ");
  scanf("%d", &student.age);
  printf ("Enter your student year: ");
  scanf("%d", &student.year);

  save_student(&student);

  return 0;
}
