#include <stdio.h>
#include <string.h>

int main() {
    char mystring[] = "hello world";

    for (int i=0; i<strlen(mystring); i++)
        printf("%c", mystring[i]);

    printf("\n");

    return 0;
}