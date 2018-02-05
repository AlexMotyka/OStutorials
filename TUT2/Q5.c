#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double euclid_dist(int x1, int y1, int x2, int y2) {
    double dist = sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
    return dist;
}

int main() {
    for (int i=0; i<10; i++) {
        int x1 = rand() % 101;
        int y1 = rand() % 101;
        int x2 = rand() % 101;
        int y2 = rand() % 101;

        double dist = euclid_dist(x1, y1, x2, y2);

        printf("Vector 1: (%d, %d)\n", x1, y1);
        printf("Vector 2: (%d, %d)\n", x2, y2);
        printf("Euclidean distance: %f\n", dist);
    }

    return 0;
}