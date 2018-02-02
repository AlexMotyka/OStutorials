#include <stdio.h>

int main(){
  char fname[20];
  int age;
  float height;

  printf ("Enter your first name: ");
  scanf("%s", fname);

  printf("Enter your age: ");
  scanf("%d", &age);

  printf("Enter your height: ");
  scanf("%f", &height);

  printf("\nYou entered:\nFirst name: %s\nAge: %d\nHeight: %f\n", fname, age, height);
  return 0;
}
