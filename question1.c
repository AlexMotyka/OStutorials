#include <stdio.h>

int main(){
  char fname[20];
  int age;
  int height;

  printf ("Enter your first name: ");
  scanf("%s", fname);

  printf("Enter your age: ");
  scanf("%d", &age);

  printf("Enter your height: ");
  scanf("%d", &height);

  printf("\nYou entered:\nFirst name: %s\nAge: %d\nHeight: %d\n", fname, age, height);
  return 0;
}
