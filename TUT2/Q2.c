#include <stdio.h>

int main() {
    double myarray[5] = {1.2, 5.5, 2.1, 3.3, 3.3};

    // From 2nd element
    for (int j=1; j<=4; j++){
        if (myarray[j] > myarray[j-1])
            printf("%.1f is greater than %.1f\n", myarray[j], myarray[j-1]);

        if (myarray[j] == myarray[j-1])
            printf("%.1f is the same as %.1f\n", myarray[j], myarray[j-1]);

        if (myarray[j] < myarray[j-1])
            printf("%.1f is less than %.1f\n", myarray[j], myarray[j-1]);
    }

    return 0;
}