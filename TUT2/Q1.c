#include <stdio.h>

int main() {
    int myarray[10];

    for (int i=1; i<=10; i++)
        myarray[i-1] = i;

    for (int j=1; j<=10; j++)
        printf("%d\n", myarray[j-1]);

    return 0;
}