#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 1000000

int main() {
    int* arr1 = (int*) malloc(N * sizeof(int));
    int* arr2 = arr1 + 10;
    free(arr2);

    return 0;
}
