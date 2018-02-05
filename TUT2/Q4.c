#include <stdio.h>

int main() {
    for (int j=1; j<=10; j++) {
        if (j % 2 != 0)
            printf("%d is odd\n", j);
        else
            printf("%d is even\n", j);
    }

    return 0;
}